#include "TimeSeriesUtility.h"
#include "TimeSeriesOutput.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace SmartMet
{
namespace TimeSeries
{
// ----------------------------------------------------------------------
/*!
 * \brief Output operator for debugging purposes
 */
// ----------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const TimeSeriesData& tsdata)
{
  try
  {
    if (boost::get<TimeSeriesPtr>(&tsdata) != nullptr)
      os << **(boost::get<TimeSeriesPtr>(&tsdata));
    else if (boost::get<TimeSeriesVectorPtr>(&tsdata) != nullptr)
      os << **(boost::get<TimeSeriesVectorPtr>(&tsdata));
    else if (boost::get<TimeSeriesGroupPtr>(&tsdata) != nullptr)
      os << **(boost::get<TimeSeriesGroupPtr>(&tsdata));

    return os;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::ostream& operator<<(std::ostream& os, const OutputData& odata)
{
  try
  {
    for (const auto& item : odata)
    {
      os << item.first << " -> " << std::endl;
      unsigned int counter = 0;
      for (const auto& item2 : item.second)
        os << "#" << counter++ << "\n" << item2 << std::endl;
    }

    return os;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

namespace
{
// ----------------------------------------------------------------------
/*!
 * \brief Remove redundant timesteps used for aggregation only from the time series
 */
// ----------------------------------------------------------------------

void erase_redundant_timesteps(TimeSeries& ts, const TimeSeriesGenerator::LocalTimeList& timesteps)
{
  try
  {
    // Fast special case exit
    if (ts.empty())
      return;

    // First check if all the timesteps are relevant. We assume both TimeSeries and LocalTimeList
    // are in sorted order.

    const auto n = ts.size();
    std::size_t valid_count = 0;
    std::vector<bool> keep_timestep(n, true);

    auto next_valid_time = timesteps.cbegin();
    const auto& last_valid_time = timesteps.cend();
    for (std::size_t i = 0; i < n; i++)
    {
      const auto& data = ts[i];

      // Skip valid times until data_time is greater than or equal to it
      while (next_valid_time != last_valid_time && data.time > *next_valid_time)
        ++next_valid_time;

      // Now the time is either valid (==) or not needed
      if (next_valid_time == last_valid_time || *next_valid_time != data.time)
        keep_timestep[i] = false;
      else
      {
        ++valid_count;
        ++next_valid_time;
      }
    }

    // Quick exit if nothing needs to be erased
    if (valid_count == n)
      return;

    // Create reduced timeseries and swap it with the input
    TimeSeries output(ts.getLocalTimePool());
    output.reserve(valid_count);

    for (std::size_t i = 0; i < n; i++)
    {
      if (keep_timestep[i])
        output.push_back(ts[i]);
    }

    std::swap(ts, output);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // namespace

// ----------------------------------------------------------------------
/*!
 * \brief Erase timesteps used for aggregation only
 */
// ----------------------------------------------------------------------

TimeSeriesPtr erase_redundant_timesteps(TimeSeriesPtr ts,
                                        const TimeSeriesGenerator::LocalTimeList& timesteps)
{
  try
  {
    erase_redundant_timesteps(*ts, timesteps);
    return ts;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Erase timesteps used for aggregation only
 */
// ----------------------------------------------------------------------

TimeSeriesVectorPtr erase_redundant_timesteps(TimeSeriesVectorPtr tsv,
                                              const TimeSeriesGenerator::LocalTimeList& timesteps)
{
  try
  {
    for (auto& tv : *tsv)
      erase_redundant_timesteps(tv, timesteps);

    return tsv;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Erase timesteps used for aggregation only
 */
// ----------------------------------------------------------------------

TimeSeriesGroupPtr erase_redundant_timesteps(TimeSeriesGroupPtr tsg,
                                             const TimeSeriesGenerator::LocalTimeList& timesteps)
{
  try
  {
    for (auto& ts : *tsg)
      erase_redundant_timesteps(ts.timeseries, timesteps);
    return tsg;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

size_t number_of_elements(const OutputData& outputData)
{
  try
  {
    size_t ret = 0;
    for (const auto& output : outputData)
    {
      const std::vector<TimeSeriesData>& outdata = output.second;

      // iterate columns (parameters)
      for (unsigned int j = 0; j < outdata.size(); j++)
      {
        const TimeSeriesData& tsdata = outdata[j];

        if (boost::get<TimeSeriesPtr>(&tsdata))
        {
          TimeSeriesPtr ts = *(boost::get<TimeSeriesPtr>(&tsdata));
          if (ts && !ts->empty())
            ret += ts->size();
        }
        else if (boost::get<TimeSeriesVectorPtr>(&tsdata))
        {
          TimeSeriesVectorPtr tsv = *(boost::get<TimeSeriesVectorPtr>(&tsdata));
          if (tsv)
            for (unsigned int k = 0; k < tsv->size(); k++)
              ret += tsv->at(k).size();
        }
        else if (boost::get<TimeSeriesGroupPtr>(&tsdata))
        {
          TimeSeriesGroupPtr tsg = *(boost::get<TimeSeriesGroupPtr>(&tsdata));
          if (tsg)
            for (unsigned int k = 0; k < tsg->size(); k++)
              ret += tsg->at(k).timeseries.size();
        }
      }
    }
    return ret;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

int get_fmisid_value(const Value& value)
{
  try
  {
    // fmisid can be std::string or double
    if (boost::get<std::string>(&value))
    {
      std::string fmisidstr = boost::get<std::string>(value);
      boost::algorithm::trim(fmisidstr);
      if (!fmisidstr.empty())
        return std::stoi(fmisidstr);

      throw Fmi::Exception(BCP, "fmisid value is an empty string");
    }
    if (boost::get<int>(&value))
      return boost::get<int>(value);
    if (boost::get<double>(&value))
      return boost::get<double>(value);
    if (boost::get<None>(&value))
      throw Fmi::Exception(BCP, "Station with null fmisid encountered!");
    if (boost::get<LonLat>(&value))
      throw Fmi::Exception(BCP, "Station with latlon as fmisid encountered!");

    throw Fmi::Exception(BCP, "Unknown fmisid type");
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

int get_fmisid_value(const TimeSeries& ts)
{
  for (const auto& tv : ts)
  {
    try
    {
      return get_fmisid_value(tv.value);
    }
    catch (...)
    {
    }
  }
  return -1;
}


void add_missing_timesteps(TimeSeries& ts, const TimeSeriesGeneratorCache::TimeList& tlist)
{
  if (!tlist || tlist->empty())
    return;

  TimeSeries ts2(ts.getLocalTimePool());

  auto it = tlist->begin();

  for (const auto& value : ts)
  {
    // Add missing timesteps
    while (it != tlist->end() && *it < value.time)
    {
      ts2.emplace_back(TimedValue(*it, None()));
      ++it;
    }
    ts2.emplace_back(value);
    // If list has been iterated to the end and
    // iteration time is same as observed timestep, go to next step
    if (it != tlist->end() && *it == value.time)
      ++it;
  }
  // If there are requested timesteps after last value, add them
  while (it != tlist->end())
  {
    ts2.emplace_back(TimedValue(*it, None()));
    ++it;
  }
  ts = ts2;
}

TimeSeriesByLocation get_timeseries_by_fmisid(const std::string& producer,
											  const TimeSeriesVectorPtr& observation_result,
											  const TimeSeriesGeneratorCache::TimeList& tlist,
											  int fmisid_index)
{
  try
  {
    TimeSeriesByLocation ret;

	/*
    if (UtilityFunctions::is_flash_or_mobile_producer(producer))
    {
      ret.emplace_back(make_pair(0, observation_result));
      return ret;
    }
	*/

    // find fmisid time series
    const TimeSeries& fmisid_ts = observation_result->at(fmisid_index);

    // find indexes for locations
    std::vector<std::pair<unsigned int, unsigned int>> location_indexes;

    unsigned int start_index = 0;
    unsigned int end_index = 0;
    for (unsigned int i = 1; i < fmisid_ts.size(); i++)
    {
      if (fmisid_ts[i].value == fmisid_ts[i - 1].value)
        continue;

      end_index = i;
      location_indexes.emplace_back(std::pair<unsigned int, unsigned int>(start_index, end_index));
      start_index = i;
    }
    end_index = fmisid_ts.size();
    location_indexes.emplace_back(std::pair<unsigned int, unsigned int>(start_index, end_index));

    // Iterate through locations
    for (const auto& location_index : location_indexes)
    {
      TimeSeriesVectorPtr tsv(new TimeSeriesVector());
      start_index = location_index.first;
      end_index = location_index.second;
      for (unsigned int k = 0; k < observation_result->size(); k++)
      {
        const TimeSeries& ts_k = observation_result->at(k);
        if (ts_k.empty())
          tsv->push_back(ts_k);
        else
        {
          TimeSeries ts_ik(fmisid_ts.getLocalTimePool());
          ts_ik.insert(ts_ik.begin(), ts_k.begin() + start_index, ts_k.begin() + end_index);
          // Add missing timesteps
          add_missing_timesteps(ts_ik, tlist);
          tsv->emplace_back(ts_ik);
        }
      }

      if (fmisid_ts.empty())
        continue;
      int fmisid = get_fmisid_value(fmisid_ts[start_index].value);
      ret.emplace_back(make_pair(fmisid, tsv));
    }

    return ret;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}



}  // namespace TimeSeries
}  // namespace SmartMet
