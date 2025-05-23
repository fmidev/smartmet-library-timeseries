#include "TimeSeriesAggregator.h"

#include "Stat.h"
#include "TimeSeries.h"
#include "TimeSeriesOutput.h"
#include "TimeSeriesUtility.h"
#include <boost/cast.hpp>
#include <macgyver/Exception.h>
#include <macgyver/StringConversion.h>
#include <newbase/NFmiGlobals.h>
#include <spine/LonLat.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>

using namespace std;
using SmartMet::Spine::LonLat;
using SmartMet::Spine::None;

using time_duration = Fmi::TimeDuration;

// #define MYDEBUG 1

namespace SmartMet
{
namespace TimeSeries
{
namespace Aggregator
{

class StatCalculator
{
 private:
  // doubles into itsDataVector, other types into itsTimeSeries
  // Note! NaN values are always put into itsTimeSeries, because they are
  // strings, so there can be data in both vectors
  Stat::DataVector itsDataVector;
  TimeSeries itsTimeSeries;

  double getDoubleStatValue(const DataFunction &func, bool useWeights) const;
  std::string getStringStatValue(const DataFunction &func) const;
  Fmi::LocalDateTime getLocalDateTimeStatValue(const DataFunction &func) const;
  LonLat getLonLatStatValue(const DataFunction &func) const;

  std::optional<Fmi::LocalDateTime> itsTimestep;

 public:
  StatCalculator() = default;
  void operator()(const TimedValue &tv);
  Value getStatValue(const DataFunction &func, bool useWeights) const;
  void setTimestep(const Fmi::LocalDateTime &timestep) { itsTimestep = timestep; }
};

namespace
{
bool include_value(const TimedValue &tv, const DataFunction &func)
{
  bool ret = true;

  FunctionId funcId = func.id();

  if (func.lowerOrUpperLimitGiven() && funcId != FunctionId::Percentage &&
      funcId != FunctionId::Count)
  {
    std::optional<double> double_value;
    if (const double *tmp = std::get_if<double>(&tv.value))
    {
      double_value = *tmp;
    }
    else if (const int *tmp = std::get_if<int>(&tv.value))
    {
      double_value = *tmp;
    }

    if (double_value && (*double_value < func.lowerLimit() || *double_value > func.upperLimit()))
      ret = false;
  }

  return ret;
}

TimeSeries area_aggregate(const TimeSeriesGroup &ts_group, const DataFunction &func)
{
  try
  {
    TimeSeries ret;

    if (ts_group.empty())
    {
      return ret;
    }

    // all time series in the result container are same length, so we use length
    // of the first
    size_t ts_size(ts_group[0].timeseries.size());

    // iterate through timesteps
    for (size_t i = 0; i < ts_size; i++)
    {
      StatCalculator statcalculator;

      // iterate through locations
      for (const auto &t : ts_group)
      {
        const TimedValue &tv = t.timeseries[i];
        if (include_value(tv, func))
          statcalculator(tv);
      }
      // take timestamps from first location (they are same for all locations
      // inside area)
      const Fmi::LocalDateTime &timestamp = ts_group[0].timeseries[i].time;

      ret.emplace_back(TimedValue(timestamp, statcalculator.getStatValue(func, false)));
    }

    return ret;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// Aggregate only to one timestep (UNUSED FUNCTION)
#if 0
TimedValue time_aggregate(const TimeSeries &ts,
                          const DataFunction &func,
                          const Fmi::LocalDateTime &timestep)
{
  try
  {
    StatCalculator statcalculator;
    statcalculator.setTimestep(timestep);

    auto start_time =
        (timestep.utc_time() - Fmi::Minutes(func.getAggregationIntervalBehind()));
    auto end_time =
        (timestep.utc_time() + Fmi::Minutes(func.getAggregationIntervalAhead()));
    Fmi::TimePeriod aggregation_period(start_time, end_time);

    for (std::size_t i = 0; i < ts.size(); i++)
    {
      const TimedValue &tv = ts.at(i);

      if (aggregation_period.contains(tv.time.utc_time()) && include_value(tv, func))
        statcalculator(tv);
    }

    return {timestep, statcalculator.getStatValue(func, true)};
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}
#endif

}  // namespace

double StatCalculator::getDoubleStatValue(const DataFunction &func, bool useWeights) const
{
  try
  {
    const double kDoubleMissing = kFloatMissing;

    Stat::Stat stat(itsDataVector, kDoubleMissing);
    stat.useWeights(useWeights);
    stat.useDegrees(func.isDirFunction());

    switch (func.id())
    {
      case FunctionId::Mean:
        return stat.mean();
      case FunctionId::Amean:
      {
        // No weights, just arithmetic mean
        stat.useWeights(false);
        return stat.mean();
      }
      case FunctionId::CircleMean:
        return stat.circlemean();
      case FunctionId::Maximum:
        return stat.max();
      case FunctionId::Minimum:
        return stat.min();
      case FunctionId::Median:
        return stat.median();
      case FunctionId::Sum:
        stat.useWeights(false);
        return stat.sum();
      case FunctionId::Integ:
        return stat.integ();
      case FunctionId::StandardDeviation:
        return stat.stddev();
      case FunctionId::Percentage:
        return stat.percentage(func.lowerLimit(), func.upperLimit());
      case FunctionId::Count:
        return stat.count(func.lowerLimit(), func.upperLimit());
      case FunctionId::Change:
        return stat.change();
      case FunctionId::Trend:
        return stat.trend();
      case FunctionId::Nearest:
        return (itsTimestep ? stat.nearest(itsTimestep->utc_time()) : kDoubleMissing);
      case FunctionId::Interpolate:
        return (itsTimestep ? stat.interpolate(itsTimestep->utc_time()) : kDoubleMissing);
      case FunctionId::NullFunction:
        return kDoubleMissing;
    }
    // NOTREACHED
    return kDoubleMissing;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::string StatCalculator::getStringStatValue(const DataFunction &func) const
{
  try
  {
    FunctionId fid(func.id());

    switch (fid)
    {
      case FunctionId::Mean:
      case FunctionId::Amean:
      case FunctionId::StandardDeviation:
      case FunctionId::Percentage:
      case FunctionId::Change:
      case FunctionId::Trend:
        return std::get<std::string>(itsTimeSeries[0].value);

      case FunctionId::Maximum:
        return std::get<std::string>(itsTimeSeries[itsTimeSeries.size() - 1].value);

      case FunctionId::Minimum:
        return std::get<std::string>(itsTimeSeries[0].value);

      case FunctionId::Median:
        return std::get<std::string>(itsTimeSeries[itsTimeSeries.size() / 2].value);

      case FunctionId::Nearest:
      case FunctionId::Interpolate:
      {
        if (itsTimestep)
        {
          for (auto item : itsTimeSeries)
            if (item.time.utc_time() == itsTimestep->utc_time())
              return std::get<std::string>(item.value);
        }
        return std::get<std::string>(itsTimeSeries[0].value);
      }

      case FunctionId::Sum:
      case FunctionId::Integ:
      {
        std::stringstream ss;
        ss << "[";
        for (const TimedValue &tv : itsTimeSeries)
        {
          if (ss.str().size() > 1)
            ss << " ";
          ss << std::get<std::string>(tv.value);
        }
        ss << "]";
        return ss.str();
      }

      case FunctionId::Count:
      {
        // Stat::Count functions can not be applid to strings, so
        // first add timesteps into data vector with double value 1.0,
        // then call Stat::count-function
        Stat::DataVector dataVector;
        for (const auto &item : itsTimeSeries)
          dataVector.emplace_back(Stat::DataItem(item.time.utc_time(), 1.0));
        Stat::Stat stat(dataVector, kFloatMissing);
        return Fmi::to_string(stat.count(func.lowerLimit(), func.upperLimit()));
      }
      default:
      {
        std::stringstream ss;
        ss << "Function " << func.hash() << " can not be applied for a string!";
        throw Fmi::Exception(BCP, ss.str());
      }
    }
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

Fmi::LocalDateTime StatCalculator::getLocalDateTimeStatValue(const DataFunction &func) const
{
  try
  {
    FunctionId fid(func.id());

    if (fid == FunctionId::Maximum)
      return std::get<Fmi::LocalDateTime>(itsTimeSeries[itsTimeSeries.size() - 1].value);

    if (fid == FunctionId::Minimum)
      return std::get<Fmi::LocalDateTime>(itsTimeSeries[0].value);

    if (fid == FunctionId::Median)
      return std::get<Fmi::LocalDateTime>(itsTimeSeries[itsTimeSeries.size() / 2].value);

    std::stringstream ss;
    ss << "Function " << func.hash() << " can not be applied for a date!";
    throw Fmi::Exception(BCP, ss.str());
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

LonLat StatCalculator::getLonLatStatValue(const DataFunction &func) const
{
  try
  {
    std::vector<double> lon_vector;
    std::vector<double> lat_vector;

    for (const TimedValue &tv : itsTimeSeries)
    {
      Value value(tv.value);
      lon_vector.push_back((std::get<LonLat>(value)).lon);
      lat_vector.push_back((std::get<LonLat>(value)).lat);
    }

    FunctionId fid(func.id());

    if (fid == FunctionId::Maximum)
    {
      std::vector<double>::iterator iter;
      iter = std::max_element(lon_vector.begin(), lon_vector.end());
      double lon_max(*iter);
      iter = std::max_element(lat_vector.begin(), lat_vector.end());
      double lat_max(*iter);
      return {lon_max, lat_max};
    }
    if (fid == FunctionId::Minimum)
    {
      std::vector<double>::iterator iter;
      iter = std::min_element(lon_vector.begin(), lon_vector.end());
      double lon_min(*iter);
      iter = std::min_element(lat_vector.begin(), lat_vector.end());
      double lat_min(*iter);
      return {lon_min, lat_min};
    }
    if (fid == FunctionId::Sum || fid == FunctionId::Integ)
    {
      double lon_sum = std::accumulate(lon_vector.begin(), lon_vector.end(), 0.0);
      double lat_sum = std::accumulate(lat_vector.begin(), lat_vector.end(), 0.0);

      while (abs(lon_sum) > 180)
        lon_sum -= 180;
      while (abs(lat_sum) > 90)
        lat_sum -= 90;

      return {lon_sum, lat_sum};
    }

    std::stringstream ss;
    ss << "Function " << func.hash() << " can not be applied for a lonlat-coordinate!";
    throw Fmi::Exception(BCP, ss.str());
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

void StatCalculator::operator()(const TimedValue &tv)
{
  try
  {
    if (const double *d = std::get_if<double>(&(tv.value)))
    {
      itsDataVector.emplace_back(Stat::DataItem(tv.time.utc_time(), *d));
    }
    else if (const int *i = std::get_if<int>(&(tv.value)))
    {
      double d = *i;
      itsDataVector.emplace_back(Stat::DataItem(tv.time.utc_time(), d));
    }
    else
    {
      itsTimeSeries.push_back(tv);
    }
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

Value StatCalculator::getStatValue(const DataFunction &func, bool useWeights) const
{
  try
  {
    // if result set contains missing values, then 'nan'-flag in DataFunction
    // determines if we ignore nan values and do statistical calculations with
    // existing values or return missing value
    bool missingValuesPresent(false);
    for (const TimedValue &tv : itsTimeSeries)
    {
      if (std::get_if<None>(&(tv.value)))
      {
        missingValuesPresent = true;
        break;
      }
    }

    if (!func.isNanFunction() && missingValuesPresent)
      return None();

    Value ret = None();
    if (!itsDataVector.empty())
    {
      double result = getDoubleStatValue(func, useWeights);
      if (result == kFloatMissing &&
          (func.id() == FunctionId::Nearest || func.id() == FunctionId::Interpolate))
        return None();
      ret = getDoubleStatValue(func, useWeights);
    }
    else if (!itsTimeSeries.empty())
    {
      Value value(itsTimeSeries[0].value);

      if (std::get_if<std::string>(&value))
      {
        if (!std::get_if<None>(&value))
        {
          ret = getStringStatValue(func);
        }
      }
      else if (std::get_if<Fmi::LocalDateTime>(&value))
      {
        ret = getLocalDateTimeStatValue(func);
      }
      else if (std::get_if<LonLat>(&value))
      {
        ret = getLonLatStatValue(func);
      }
    }
    return ret;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}


TimeSeriesPtr time_aggregate(
        const TimeSeries& ts,
        const DataFunction& func,
        const TimeSeriesGenerator::LocalTimeList& timesteps)
try
{
  const Fmi::TimeDuration& before = Fmi::Minutes(func.getAggregationIntervalBehind());
  const Fmi::TimeDuration& after = Fmi::Minutes(func.getAggregationIntervalAhead());

  TimeSeries::const_iterator agg_begin_iter = ts.begin();
  TimeSeries::const_iterator agg_end_iter = ts.begin();

  TimeSeriesPtr ret(new TimeSeries);

  // Return empty result if input time series is empty
  if (ts.empty())
    return ret;

  for (TimeSeriesGenerator::LocalTimeList::const_iterator timestep_iter = timesteps.begin();
       timestep_iter != timesteps.end();
       ++timestep_iter)
  {
    const Fmi::LocalDateTime timestamp  = *timestep_iter;
    Fmi::LocalDateTime agg_begin = timestamp - before;
    Fmi::LocalDateTime agg_end = timestamp + after;

    agg_begin_iter = std::find_if(agg_begin_iter, ts.end(),
      [&agg_begin](const TimedValue& tv) { return tv.time >= agg_begin; });

    agg_end_iter = std::find_if(agg_end_iter, ts.end(),
      [&agg_end](const TimedValue& tv) { return tv.time > agg_end; });

    StatCalculator statcalculator;
    statcalculator.setTimestep(timestamp);

    for (TimeSeries::const_iterator it = agg_begin_iter; it != agg_end_iter; ++it)
    {
      // Be a bit paranoid and check that we don't go beyond the end of the time series
      // Should never happen, but better safe than sorry
      if (it == ts.end())
      {
        throw Fmi::Exception(BCP, "INTERNAL ERROR: Time series end reached before aggregation end");
      }
      if (include_value(*it, func))
        statcalculator(*it);
    }

    ret->emplace_back(TimedValue(timestamp, statcalculator.getStatValue(func, true)));
  }
  return ret;
}
catch(...)
{
  throw Fmi::Exception::Trace(BCP, "Operation failed!");
}


TimeSeriesGroupPtr time_aggregate(const TimeSeriesGroup &ts_group,
                                  const DataFunction &func,
                                  const TimeSeriesGenerator::LocalTimeList &timesteps)
{
  try
  {
    TimeSeriesGroupPtr ret(new TimeSeriesGroup());

    // iterate through locations
    for (const auto &t : ts_group)
    {
      TimeSeries ts(t.timeseries);
      TimeSeriesPtr aggregated_timeseries(time_aggregate(ts, func, timesteps));
      ret->emplace_back(LonLatTimeSeries(t.lonlat, *aggregated_timeseries));
    }

    return ret;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// Before only time-aggregation was possible here, but since
// filtering was added also 'area aggregation' may happen
TimeSeriesPtr aggregate(const TimeSeries& ts,
                        const DataFunctions& pf,
                        const TimeSeriesGenerator::LocalTimeList& timesteps)
try
{
  TimeSeriesPtr ret(new TimeSeries);

  if (pf.innerFunction.type() == FunctionType::AreaFunction)
  {
    TimeSeries local_ts;
    // Do filtering
    for (const auto &tv : ts)
    {
      if (include_value(tv, pf.innerFunction))
        local_ts.push_back(tv);
      else
        local_ts.emplace_back(TimedValue(tv.time, None()));
    }

    // Do time aggregationn
    if (pf.outerFunction.type() == FunctionType::TimeFunction)
    {
      ret = time_aggregate(local_ts, pf.outerFunction, timesteps);
    }
    else
    {
      *ret = local_ts;
    }
  }
  else if (pf.innerFunction.type() == FunctionType::TimeFunction)
  {
    ret = time_aggregate(ts, pf.innerFunction, timesteps);
    if (pf.outerFunction.type() == FunctionType::AreaFunction)
    {
      // Do filtering
      TimeSeries local_ts = *ret;
      ret->clear();
      for (const auto &tv : local_ts)
      {
        if (include_value(tv, pf.outerFunction))
          ret->push_back(tv);
        else
          ret->emplace_back(TimedValue(tv.time, None()));
      }
    }
  }
  else
  {
    *ret = ts;
  }

  return ret;
}
catch (...)
{
  throw Fmi::Exception::Trace(BCP, "Operation failed!");
}

TimeSeriesGroupPtr aggregate(const TimeSeriesGroup& ts_group,
                             const DataFunctions& pf,
                             const TimeSeriesGenerator::LocalTimeList& timesteps)
try
{
  TimeSeriesGroupPtr ret(new TimeSeriesGroup);

  if (ts_group.empty())
  {
    return ret;
  }

  if (pf.outerFunction.type() == FunctionType::TimeFunction &&
      pf.innerFunction.type() == FunctionType::AreaFunction)
  {
#ifdef MYDEBUG
    cout << "time-area aggregation" << endl;
#endif

    // 1) do area aggregation
    TimeSeries area_aggregated_vector = area_aggregate(ts_group, pf.innerFunction);

    // 2) do time aggregation
    TimeSeriesPtr ts = time_aggregate(area_aggregated_vector, pf.outerFunction, timesteps);

    ret->emplace_back(LonLatTimeSeries(ts_group[0].lonlat, *ts));
  }
  else if (pf.outerFunction.type() == FunctionType::AreaFunction &&
           pf.innerFunction.type() == FunctionType::TimeFunction)
  {
#ifdef MYDEBUG
    cout << "area-time aggregation" << endl;
#endif
    // 1) do time aggregation
    TimeSeriesGroupPtr time_aggregated_result = time_aggregate(ts_group, pf.innerFunction, timesteps);

    // 2) do area aggregation
    TimeSeries ts = area_aggregate(*time_aggregated_result, pf.outerFunction);

    ret->emplace_back(LonLatTimeSeries(ts_group[0].lonlat, ts));
  }
  else if (pf.innerFunction.type() == FunctionType::AreaFunction)
  {
#ifdef MYDEBUG
    cout << "area aggregation" << endl;
#endif
    // 1) do area aggregation
    TimeSeries area_aggregated_vector = area_aggregate(ts_group, pf.innerFunction);

    ret->emplace_back(LonLatTimeSeries(ts_group[0].lonlat, area_aggregated_vector));
  }
  else if (pf.innerFunction.type() == FunctionType::TimeFunction)
  {
#ifdef MYDEBUG
    cout << "time aggregation" << endl;
#endif

    // 1) do time aggregation
    ret = time_aggregate(ts_group, pf.innerFunction, timesteps);
  }
  else
  {
#ifdef MYDEBUG
    cout << "no aggregation" << endl;
#endif
    *ret = ts_group;
  }

  return ret;
}
catch (...)
{
  throw Fmi::Exception::Trace(BCP, "Operation failed!");
}

}  // namespace Aggregator
}  // namespace TimeSeries
}  // namespace SmartMet
