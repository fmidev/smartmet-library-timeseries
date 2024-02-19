// ======================================================================
/*!
 * \brief Regression tests for TimeSeriesAggregator
 */
// ======================================================================

#include "TimeSeriesInclude.h"
#include <memory>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <regression/tframe.h>

const char *tz_eet_name = "EET";

// Protection against namespace tests
namespace TimeSeriesAggregatorTest
{
//  observations
TS::TimeSeries generate_observation_timeseries()
{
  using namespace SmartMet;

  Fmi::TimeZonePtr zone(tz_eet_name);

  auto pool = std::make_shared<TS::LocalTimePool>();
  TS::TimeSeries timeseries(pool);

  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), TS::None()));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22) + Fmi::Minutes(10)),
                         zone),
      1.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22) + Fmi::Minutes(15)),
                         zone),
      2.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22) + Fmi::Minutes(48)),
                         zone),
      3.5));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), TS::None()));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23) + Fmi::Minutes(8)),
                         zone),
      4.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23) + Fmi::Minutes(52)),
                         zone),
      5.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), TS::None()));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24) + Fmi::Minutes(9)),
                         zone),
      5.5));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24) + Fmi::Minutes(53)),
                         zone),
      3.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 2.5));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25) + Fmi::Minutes(50)),
                         zone),
      -1.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), TS::None()));

  return timeseries;
}

//  direction observations
TS::TimeSeries generate_direction_timeseries()
{
  using namespace SmartMet;

  Fmi::TimeZonePtr zone(tz_eet_name);

  auto pool = std::make_shared<TS::LocalTimePool>();
  TS::TimeSeries timeseries(pool);

  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), TS::None()));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22) + Fmi::Minutes(10)),
                         zone),
      340.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22) + Fmi::Minutes(15)),
                         zone),
      350.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22) + Fmi::Minutes(48)),
                         zone),
      355.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), TS::None()));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23) + Fmi::Minutes(8)),
                         zone),
      5.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23) + Fmi::Minutes(51)),
                         zone),
      20.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), TS::None()));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24) + Fmi::Minutes(9)),
                         zone),
      10.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24) + Fmi::Minutes(53)),
                         zone),
      355.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 350.));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25) + Fmi::Minutes(50)),
                         zone),
      340.0));
  timeseries.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), TS::None()));

  return timeseries;
}

TS::TimeSeries generate_timeseries_with_diverse_values(bool add_missing_value = false,
                                                       bool degrees = false)
{
  using namespace SmartMet;

  Fmi::TimeZonePtr zone(tz_eet_name);

  auto pool = std::make_shared<TS::LocalTimePool>();
  TS::TimeSeries timeseries(pool);

  Fmi::LocalDateTime t1(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone);
  Fmi::LocalDateTime t2(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone);
  Fmi::LocalDateTime t3(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone);
  Fmi::LocalDateTime t4(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone);
  Fmi::LocalDateTime t5(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone);

  if (!degrees)
  {
    timeseries.push_back(TS::TimedValue(t1, 1.0));
    timeseries.push_back(TS::TimedValue(t2, 7.0));
    timeseries.push_back(TS::TimedValue(t3, 4.0));
    if (add_missing_value)
      timeseries.push_back(TS::TimedValue(t4, TS::None()));
    else
      timeseries.push_back(TS::TimedValue(t4, 5.0));
    timeseries.push_back(TS::TimedValue(t5, 3.0));
  }
  else
  {
    timeseries.push_back(TS::TimedValue(t1, 340.0));
    timeseries.push_back(TS::TimedValue(t2, 350.0));
    timeseries.push_back(TS::TimedValue(t3, 355.0));
    if (add_missing_value)
      timeseries.push_back(TS::TimedValue(t4, TS::None()));
    else
      timeseries.push_back(TS::TimedValue(t4, 5.0));
    timeseries.push_back(TS::TimedValue(t5, 10.0));
  }

  return timeseries;
}

TS::TimeSeries generate_timeseries(bool add_missing_value = false, bool degrees = false)
{
  using namespace SmartMet;

  Fmi::TimeZonePtr zone(tz_eet_name);

  auto pool = std::make_shared<TS::LocalTimePool>();
  TS::TimeSeries timeseries(pool);

  Fmi::LocalDateTime t1(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone);
  Fmi::LocalDateTime t2(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone);
  Fmi::LocalDateTime t3(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone);
  Fmi::LocalDateTime t4(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone);
  Fmi::LocalDateTime t5(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone);

  if (!degrees)
  {
    timeseries.push_back(TS::TimedValue(t1, 1.0));
    timeseries.push_back(TS::TimedValue(t2, 2.0));
    timeseries.push_back(TS::TimedValue(t3, 3.0));
    if (add_missing_value)
      timeseries.push_back(TS::TimedValue(t4, TS::None()));
    else
      timeseries.push_back(TS::TimedValue(t4, 4.0));
    timeseries.push_back(TS::TimedValue(t5, 5.0));
  }
  else
  {
    timeseries.push_back(TS::TimedValue(t1, 340.0));
    timeseries.push_back(TS::TimedValue(t2, 350.0));
    timeseries.push_back(TS::TimedValue(t3, 355.0));
    if (add_missing_value)
      timeseries.push_back(TS::TimedValue(t4, TS::None()));
    else
      timeseries.push_back(TS::TimedValue(t4, 5.0));
    timeseries.push_back(TS::TimedValue(t5, 10.0));
  }

  return timeseries;
}

TS::TimeSeriesGroup generate_timeseries_group()
{
  using namespace SmartMet;

  Fmi::TimeZonePtr zone(tz_eet_name);

  auto pool = std::make_shared<TS::LocalTimePool>();
  TS::TimeSeries timeseries_helsinki(pool);

  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 1.0));
  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 6.0));
  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), 11.0));
  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 16.0));
  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 21.0));

  TS::TimeSeries timeseries_tampere(pool);

  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 2.0));
  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 7.0));
  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), 12.0));
  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 17.0));
  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 22.0));

  TS::TimeSeries timeseries_oulu(pool);

  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 3.0));
  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 8.0));
  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), 13.0));
  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 18.0));
  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 23.0));

  TS::TimeSeries timeseries_kuopio(pool);

  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 4.0));
  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 9.0));
  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), 14.0));
  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 19.0));
  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 24.0));

  TS::TimeSeries timeseries_turku(pool);

  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 5.0));
  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 10.0));
  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), 15.0));
  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 20.0));
  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 25.0));

  TS::LonLatTimeSeries lonlat_ts_helsinki(TS::LonLat(24.9375, 60.1718), timeseries_helsinki);
  TS::LonLatTimeSeries lonlat_ts_tampere(TS::LonLat(23.7667, 61.5000), timeseries_tampere);
  TS::LonLatTimeSeries lonlat_ts_oulu(TS::LonLat(25.4667, 65.0167), timeseries_oulu);
  TS::LonLatTimeSeries lonlat_ts_kuopio(TS::LonLat(27.6783, 62.8925), timeseries_kuopio);
  TS::LonLatTimeSeries lonlat_ts_turku(TS::LonLat(22.2667, 60.4500), timeseries_turku);

  TS::TimeSeriesGroup timeseries_grp;
  timeseries_grp.push_back(lonlat_ts_helsinki);
  timeseries_grp.push_back(lonlat_ts_tampere);
  timeseries_grp.push_back(lonlat_ts_oulu);
  timeseries_grp.push_back(lonlat_ts_kuopio);
  timeseries_grp.push_back(lonlat_ts_turku);

  return timeseries_grp;
}

TS::TimeSeriesGroup generate_timeseries_group_nans()
{
  using namespace SmartMet;

  Fmi::TimeZonePtr zone(tz_eet_name);

  auto pool = std::make_shared<TS::LocalTimePool>();
  TS::TimeSeries timeseries_helsinki(pool);

  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 1.0));
  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 6.0));
  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), TS::None()));
  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 16.0));
  timeseries_helsinki.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 21.0));

  TS::TimeSeries timeseries_tampere(pool);

  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 2.0));
  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 7.0));
  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), 17.0));
  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), TS::None()));
  timeseries_tampere.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 22.0));

  TS::TimeSeries timeseries_oulu(pool);

  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 3.0));
  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 8.0));
  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), 13.0));
  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 18.0));
  timeseries_oulu.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 23.0));

  TS::TimeSeries timeseries_kuopio(pool);

  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 4.0));
  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 9.0));
  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), 14.0));
  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 19.0));
  timeseries_kuopio.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 24.0));

  TS::TimeSeries timeseries_turku(pool);

  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(22)), zone), 5.0));
  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(23)), zone), 10.0));
  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(24)), zone), 15.0));
  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(25)), zone), 20.0));
  timeseries_turku.push_back(TS::TimedValue(
      Fmi::LocalDateTime(Fmi::DateTime(Fmi::Date(2015, 3, 2), Fmi::Hours(26)), zone), 25.0));

  TS::LonLatTimeSeries lonlat_ts_helsinki(TS::LonLat(24.9375, 60.1718), timeseries_helsinki);
  TS::LonLatTimeSeries lonlat_ts_tampere(TS::LonLat(23.7667, 61.5000), timeseries_tampere);
  TS::LonLatTimeSeries lonlat_ts_oulu(TS::LonLat(25.4667, 65.0167), timeseries_oulu);
  TS::LonLatTimeSeries lonlat_ts_kuopio(TS::LonLat(27.6783, 62.8925), timeseries_kuopio);
  TS::LonLatTimeSeries lonlat_ts_turku(TS::LonLat(22.2667, 60.4500), timeseries_turku);

  TS::TimeSeriesGroup timeseries_grp;
  timeseries_grp.push_back(lonlat_ts_helsinki);
  timeseries_grp.push_back(lonlat_ts_tampere);
  timeseries_grp.push_back(lonlat_ts_oulu);
  timeseries_grp.push_back(lonlat_ts_kuopio);
  timeseries_grp.push_back(lonlat_ts_turku);

  return timeseries_grp;
}

template <typename T>
std::string to_string(const T& result_set)
{
  std::stringstream ret_ss;

  ret_ss << result_set;

  return ret_ss.str();
}

TS::TimeSeriesPtr execute_time_area_aggregation_function_with_range(TS::FunctionId fid_time,
                                                                    TS::FunctionId fid_area,
                                                                    unsigned int aggIntervalBehind,
                                                                    unsigned int aggIntervalAhead,
                                                                    double lowerLimit,
                                                                    double upperLimit,
                                                                    bool add_missing_value = false)
{
  using namespace TS;

  TimeSeries timeseries(generate_timeseries(add_missing_value));

  DataFunction pfInner(fid_time, FunctionType::TimeFunction, lowerLimit, upperLimit);
  pfInner.setAggregationIntervalBehind(aggIntervalBehind);
  pfInner.setAggregationIntervalAhead(aggIntervalAhead);
  DataFunctions pfs;
  pfs.innerFunction = pfInner;
  DataFunction pfOuter(fid_area, FunctionType::AreaFunction);
  pfs.outerFunction = pfOuter;

  TimeSeriesPtr aggregated_timeseries = Aggregator::aggregate(timeseries, pfs);

  return aggregated_timeseries;
}

TS::TimeSeriesPtr execute_area_time_aggregation_function_with_range(TS::FunctionId fid_time,
                                                                    TS::FunctionId fid_area,
                                                                    unsigned int aggIntervalBehind,
                                                                    unsigned int aggIntervalAhead,
                                                                    double lowerLimit,
                                                                    double upperLimit,
                                                                    bool add_missing_value = false)
{
  using namespace TS;

  TimeSeries timeseries(generate_timeseries(add_missing_value));

  DataFunction pfInner(fid_area, FunctionType::AreaFunction, lowerLimit, upperLimit);
  DataFunctions pfs;
  pfs.innerFunction = pfInner;
  DataFunction pfOuter(fid_time, FunctionType::TimeFunction);
  pfOuter.setAggregationIntervalBehind(aggIntervalBehind);
  pfOuter.setAggregationIntervalAhead(aggIntervalAhead);
  pfs.outerFunction = pfOuter;

  TimeSeriesPtr aggregated_timeseries = Aggregator::aggregate(timeseries, pfs);

  return aggregated_timeseries;
}

TS::TimeSeriesPtr execute_time_aggregation_function_with_range(TS::FunctionId fid,
                                                               unsigned int aggIntervalBehind,
                                                               unsigned int aggIntervalAhead,
                                                               double lowerLimit,
                                                               double upperLimit,
                                                               bool add_missing_value = false)
{
  using namespace TS;

  TimeSeries timeseries(generate_timeseries(add_missing_value));

  DataFunction pf(fid, FunctionType::TimeFunction, lowerLimit, upperLimit);
  pf.setAggregationIntervalBehind(aggIntervalBehind);
  pf.setAggregationIntervalAhead(aggIntervalAhead);
  DataFunctions pfs;
  pfs.innerFunction = pf;

  TimeSeriesPtr aggregated_timeseries = Aggregator::aggregate(timeseries, pfs);

  return aggregated_timeseries;
}

TS::TimeSeriesPtr execute_time_aggregation_function(TS::FunctionId fid,
                                                    unsigned int aggIntervalBehind,
                                                    unsigned int aggIntervalAhead,
                                                    bool add_missing_value = false,
                                                    bool degrees = false)
{
  using namespace TS;

  TimeSeries timeseries(generate_timeseries(add_missing_value, degrees));

  DataFunction pf(fid, FunctionType::TimeFunction);
  pf.setAggregationIntervalBehind(aggIntervalBehind);
  pf.setAggregationIntervalAhead(aggIntervalAhead);
  pf.setIsDirFunction(degrees);
  // lower limit (3) and upper limit (4) are used by Percentage and Count functions
  if (fid == FunctionId::Percentage || fid == FunctionId::Count)
    pf.setLimits(3.0, 4.0);
  DataFunctions pfs;
  pfs.innerFunction = pf;

  TimeSeriesPtr aggregated_timeseries = Aggregator::aggregate(timeseries, pfs);

  return aggregated_timeseries;
}

TS::TimeSeriesPtr execute_time_aggregation_function_with_diverse_values(
    TS::FunctionId fid,
    unsigned int aggIntervalBehind,
    unsigned int aggIntervalAhead,
    bool add_missing_value = false)
{
  using namespace TS;
  TimeSeries timeseries(generate_timeseries_with_diverse_values(add_missing_value));

  DataFunction pf(fid, FunctionType::TimeFunction);
  pf.setAggregationIntervalBehind(aggIntervalBehind);
  pf.setAggregationIntervalAhead(aggIntervalAhead);
  DataFunctions pfs;
  pfs.innerFunction = pf;

  TimeSeriesPtr aggregated_timeseries = Aggregator::aggregate(timeseries, pfs);

  return aggregated_timeseries;
}

TS::TimeSeriesGroupPtr execute_area_aggregation_function(TS::FunctionId fid)
{
  using namespace TS;

  TimeSeriesGroup timeseries_grp(generate_timeseries_group());

  DataFunction pf(fid, FunctionType::AreaFunction);
  // lower limit (14) and upper limit (22) are used by Percentage and Count functions
  if (fid == FunctionId::Percentage || fid == FunctionId::Count)
    pf.setLimits(14.0, 22.0);

  DataFunctions pfs;
  pfs.innerFunction = pf;
  Value missing_value("nan");

  TimeSeriesGroupPtr aggregated_timeseries_grp = Aggregator::aggregate(timeseries_grp, pfs);

  return aggregated_timeseries_grp;
}

TS::TimeSeriesGroupPtr execute_area_aggregation_function_with_range(TS::FunctionId fid,
                                                                    double lowerLimit,
                                                                    double upperLimit)
{
  using namespace TS;

  TimeSeriesGroup timeseries_grp(generate_timeseries_group());

  DataFunction pf(fid, FunctionType::AreaFunction, lowerLimit, upperLimit);
  DataFunctions pfs;
  pfs.innerFunction = pf;
  Value missing_value("nan");

  TimeSeriesGroupPtr aggregated_timeseries_grp = Aggregator::aggregate(timeseries_grp, pfs);

  return aggregated_timeseries_grp;
}

TS::TimeSeriesGroupPtr execute_area_aggregation_function_nans(TS::FunctionId fid)
{
  using namespace TS;

  TimeSeriesGroup timeseries_grp(generate_timeseries_group_nans());

  DataFunction pf(fid, FunctionType::AreaFunction);
  // lower limit (14) and upper limit (22) are used by Percentage and Count functions
  if (fid == FunctionId::Percentage || fid == FunctionId::Count)
    pf.setLimits(14.0, 22.0);
  DataFunctions pfs;
  pfs.innerFunction = pf;
  Value missing_value("nan");

  TimeSeriesGroupPtr aggregated_timeseries_grp = Aggregator::aggregate(timeseries_grp, pfs);

  return aggregated_timeseries_grp;
}

TS::TimeSeriesGroupPtr execute_area_aggregation_function_with_range_nans(TS::FunctionId fid,
                                                                         double lowerLimit,
                                                                         double upperLimit)
{
  using namespace TS;

  TimeSeriesGroup timeseries_grp(generate_timeseries_group_nans());

  DataFunction pf(fid, FunctionType::AreaFunction, lowerLimit, upperLimit);
  DataFunctions pfs;
  pfs.innerFunction = pf;
  Value missing_value("nan");

  TimeSeriesGroupPtr aggregated_timeseries_grp = Aggregator::aggregate(timeseries_grp, pfs);

  return aggregated_timeseries_grp;
}

TS::TimeSeriesGroupPtr execute_time_area_aggregation_function(TS::FunctionId time_fid,
                                                              TS::FunctionId area_fid,
                                                              unsigned int aggIntervalBehind,
                                                              unsigned int aggIntervalAhead)
{
  using namespace TS;

  TimeSeriesGroup timeseries_grp(generate_timeseries_group());

  DataFunction time_pf(time_fid, FunctionType::TimeFunction);
  time_pf.setAggregationIntervalBehind(aggIntervalBehind);
  time_pf.setAggregationIntervalAhead(aggIntervalAhead);
  // lower limit (3) and upper limit (4) are used by Percentage and Count functions
  if (time_fid == FunctionId::Percentage || time_fid == FunctionId::Count)
    time_pf.setLimits(3.0, 4.0);

  DataFunctions time_pfs;
  time_pfs.innerFunction = time_pf;
  Value missing_value("nan");

  TimeSeriesGroupPtr time_aggregated_grp = Aggregator::aggregate(timeseries_grp, time_pfs);

  DataFunction area_pf(area_fid, FunctionType::AreaFunction);
  // lower limit (14) and upper limit (22) are used by Percentage and Count functions
  if (area_fid == FunctionId::Percentage || area_fid == FunctionId::Count)
    area_pf.setLimits(14.0, 22.0);
  DataFunctions area_pfs;
  area_pfs.innerFunction = area_pf;

  TimeSeriesGroupPtr area_aggregated_grp = Aggregator::aggregate(*time_aggregated_grp, area_pfs);

  return area_aggregated_grp;
}

TS::TimeSeriesGroupPtr execute_area_time_aggregation_function(TS::FunctionId area_fid,
                                                              TS::FunctionId time_fid,
                                                              unsigned int aggIntervalBehind,
                                                              unsigned int aggIntervalAhead)
{
  using namespace TS;

  TimeSeriesGroup timeseries_grp(generate_timeseries_group());
  Value missing_value("nan");

  DataFunction area_pf(area_fid, FunctionType::AreaFunction);
  // lower limit (14) and upper limit (22) are used by Percentage and Count functions
  if (area_fid == FunctionId::Percentage || area_fid == FunctionId::Count)
    area_pf.setLimits(14.0, 22.0);

  DataFunctions area_pfs;
  area_pfs.innerFunction = area_pf;

  TimeSeriesGroupPtr area_aggregated_grp = Aggregator::aggregate(timeseries_grp, area_pfs);

  // lower limit (3) and upper limit (4) are used by Percentage and Count functions
  DataFunction time_pf(time_fid, FunctionType::TimeFunction);
  if (time_fid == FunctionId::Percentage || time_fid == FunctionId::Count)
    time_pf.setLimits(3.0, 4.0);

  time_pf.setAggregationIntervalBehind(aggIntervalBehind);
  time_pf.setAggregationIntervalAhead(aggIntervalAhead);
  DataFunctions time_pfs;
  time_pfs.innerFunction = time_pf;

  TimeSeriesGroupPtr time_aggregated_grp = Aggregator::aggregate(*area_aggregated_grp, time_pfs);

  return time_aggregated_grp;
}

void nearest_t()
{
  using namespace TS;

  Fmi::TimeZonePtr zone(tz_eet_name);

  TimeSeries timeseries = generate_observation_timeseries();

  // Replace None with the nearest valid value within +n/-n Fmi::Minutes
  DataFunction pf(FunctionId::Nearest, FunctionType::TimeFunction);
  pf.setAggregationIntervalAhead(8);
  pf.setAggregationIntervalBehind(8);

  DataFunctions pfs;
  pfs.innerFunction = pf;
  std::string test_result = to_string(*Aggregator::aggregate(timeseries, pfs));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> nan\n"
      "2015-Mar-03 00:10:00 EET -> 1\n"
      "2015-Mar-03 00:15:00 EET -> 2\n"
      "2015-Mar-03 00:48:00 EET -> 3.5\n"
      "2015-Mar-03 01:00:00 EET -> 4\n"
      "2015-Mar-03 01:08:00 EET -> 4\n"
      "2015-Mar-03 01:52:00 EET -> 5\n"
      "2015-Mar-03 02:00:00 EET -> 5\n"
      "2015-Mar-03 02:09:00 EET -> 5.5\n"
      "2015-Mar-03 02:53:00 EET -> 3\n"
      "2015-Mar-03 03:00:00 EET -> 2.5\n"
      "2015-Mar-03 03:50:00 EET -> -1\n"
      "2015-Mar-03 04:00:00 EET -> nan\n";

  if (expected_result != test_result)
    TEST_FAILED("Nearest-function test (8 min) failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  pf.setAggregationIntervalAhead(10);
  pf.setAggregationIntervalBehind(10);
  pfs.innerFunction = pf;

  test_result = to_string(*Aggregator::aggregate(timeseries, pfs));

  expected_result =
      "2015-Mar-03 00:00:00 EET -> 1\n"
      "2015-Mar-03 00:10:00 EET -> 1\n"
      "2015-Mar-03 00:15:00 EET -> 2\n"
      "2015-Mar-03 00:48:00 EET -> 3.5\n"
      "2015-Mar-03 01:00:00 EET -> 4\n"
      "2015-Mar-03 01:08:00 EET -> 4\n"
      "2015-Mar-03 01:52:00 EET -> 5\n"
      "2015-Mar-03 02:00:00 EET -> 5\n"
      "2015-Mar-03 02:09:00 EET -> 5.5\n"
      "2015-Mar-03 02:53:00 EET -> 3\n"
      "2015-Mar-03 03:00:00 EET -> 2.5\n"
      "2015-Mar-03 03:50:00 EET -> -1\n"
      "2015-Mar-03 04:00:00 EET -> -1\n";

  if (expected_result != test_result)
    TEST_FAILED("Nearest-function test (10 min) failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void interpolate_t()
{
  using namespace TS;

  Fmi::TimeZonePtr zone(tz_eet_name);

  TimeSeries timeseries = generate_observation_timeseries();

  // Replace None with the interpolated value using valid values within +n/-n Fmi::Minutes
  DataFunction pf(FunctionId::Interpolate, FunctionType::TimeFunction);
  pf.setAggregationIntervalAhead(10);
  pf.setAggregationIntervalBehind(10);

  DataFunctions pfs;
  pfs.innerFunction = pf;
  std::string test_result = to_string(*Aggregator::aggregate(timeseries, pfs));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> nan\n"
      "2015-Mar-03 00:10:00 EET -> 1\n"
      "2015-Mar-03 00:15:00 EET -> 2\n"
      "2015-Mar-03 00:48:00 EET -> 3.5\n"
      "2015-Mar-03 01:00:00 EET -> nan\n"
      "2015-Mar-03 01:08:00 EET -> 4\n"
      "2015-Mar-03 01:52:00 EET -> 5\n"
      "2015-Mar-03 02:00:00 EET -> 5.23529\n"
      "2015-Mar-03 02:09:00 EET -> 5.5\n"
      "2015-Mar-03 02:53:00 EET -> 3\n"
      "2015-Mar-03 03:00:00 EET -> 2.5\n"
      "2015-Mar-03 03:50:00 EET -> -1\n"
      "2015-Mar-03 04:00:00 EET -> nan\n";

  if (expected_result != test_result)
    TEST_FAILED("Interpolate-function test (10 min) failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  pf.setAggregationIntervalAhead(15);
  pf.setAggregationIntervalBehind(15);
  pfs.innerFunction = pf;

  test_result = to_string(*Aggregator::aggregate(timeseries, pfs));

  expected_result =
      "2015-Mar-03 00:00:00 EET -> -1\n"
      "2015-Mar-03 00:10:00 EET -> 1\n"
      "2015-Mar-03 00:15:00 EET -> 2\n"
      "2015-Mar-03 00:48:00 EET -> 3.5\n"
      "2015-Mar-03 01:00:00 EET -> 3.8\n"
      "2015-Mar-03 01:08:00 EET -> 4\n"
      "2015-Mar-03 01:52:00 EET -> 5\n"
      "2015-Mar-03 02:00:00 EET -> 5.23529\n"
      "2015-Mar-03 02:09:00 EET -> 5.5\n"
      "2015-Mar-03 02:53:00 EET -> 3\n"
      "2015-Mar-03 03:00:00 EET -> 2.5\n"
      "2015-Mar-03 03:50:00 EET -> -1\n"
      "2015-Mar-03 04:00:00 EET -> nan\n";

  if (expected_result != test_result)
    TEST_FAILED("Interpolate-function test (15 min) failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void interpolatedir_t()
{
  using namespace TS;

  Fmi::TimeZonePtr zone(tz_eet_name);

  TimeSeries timeseries = generate_direction_timeseries();

  // Replace None with the interpolated value using valid values within +n/-n Fmi::Minutes
  DataFunction pf(FunctionId::Interpolate, FunctionType::TimeFunction);
  pf.setAggregationIntervalAhead(10);
  pf.setAggregationIntervalBehind(10);
  pf.setIsDirFunction(true);

  DataFunctions pfs;
  pfs.innerFunction = pf;
  std::string test_result = to_string(*Aggregator::aggregate(timeseries, pfs));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> nan\n"
      "2015-Mar-03 00:10:00 EET -> 340\n"
      "2015-Mar-03 00:15:00 EET -> 350\n"
      "2015-Mar-03 00:48:00 EET -> 355\n"
      "2015-Mar-03 01:00:00 EET -> nan\n"
      "2015-Mar-03 01:08:00 EET -> 5\n"
      "2015-Mar-03 01:51:00 EET -> 20\n"
      "2015-Mar-03 02:00:00 EET -> 15\n"
      "2015-Mar-03 02:09:00 EET -> 10\n"
      "2015-Mar-03 02:53:00 EET -> 355\n"
      "2015-Mar-03 03:00:00 EET -> 350\n"
      "2015-Mar-03 03:50:00 EET -> 340\n"
      "2015-Mar-03 04:00:00 EET -> nan\n";

  if (expected_result != test_result)
    TEST_FAILED("InterpolateDir-function test (10 min) failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  pf.setAggregationIntervalAhead(15);
  pf.setAggregationIntervalBehind(15);
  pfs.innerFunction = pf;

  test_result = to_string(*Aggregator::aggregate(timeseries, pfs));

  expected_result =
      "2015-Mar-03 00:00:00 EET -> 320\n"
      "2015-Mar-03 00:10:00 EET -> 340\n"
      "2015-Mar-03 00:15:00 EET -> 350\n"
      "2015-Mar-03 00:48:00 EET -> 355\n"
      "2015-Mar-03 01:00:00 EET -> 1\n"
      "2015-Mar-03 01:08:00 EET -> 5\n"
      "2015-Mar-03 01:51:00 EET -> 20\n"
      "2015-Mar-03 02:00:00 EET -> 15\n"
      "2015-Mar-03 02:09:00 EET -> 10\n"
      "2015-Mar-03 02:53:00 EET -> 355\n"
      "2015-Mar-03 03:00:00 EET -> 350\n"
      "2015-Mar-03 03:50:00 EET -> 340\n"
      "2015-Mar-03 04:00:00 EET -> nan\n";

  if (expected_result != test_result)
    TEST_FAILED("InterpolatDir-function test (15 min) failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void min_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Minimum, 120, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 1\n"
      "2015-Mar-03 01:00:00 EET -> 1\n"
      "2015-Mar-03 02:00:00 EET -> 1\n"
      "2015-Mar-03 03:00:00 EET -> 2\n"
      "2015-Mar-03 04:00:00 EET -> 3\n";

  if (test_result != expected_result)
    TEST_FAILED("Minimum-function test failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void min_t_with_range()
{
  // Include 2,3
  std::string test_result = to_string(
      *execute_time_aggregation_function_with_range(TS::FunctionId::Minimum, 120, 0, 2.0, 3.0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> nan\n"
      "2015-Mar-03 01:00:00 EET -> 2\n"
      "2015-Mar-03 02:00:00 EET -> 2\n"
      "2015-Mar-03 03:00:00 EET -> 2\n"
      "2015-Mar-03 04:00:00 EET -> 3\n";

  if (test_result != expected_result)
    TEST_FAILED("Minimum-function with range test failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void max_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Maximum, 0, 120));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 3\n"
      "2015-Mar-03 01:00:00 EET -> 4\n"
      "2015-Mar-03 02:00:00 EET -> 5\n"
      "2015-Mar-03 03:00:00 EET -> 5\n"
      "2015-Mar-03 04:00:00 EET -> 5\n";

  if (expected_result != test_result)
    TEST_FAILED("Max-function test failed. Result should be:\n" + expected_result + "\n not \n" +
                test_result);

  TEST_PASSED();
}

void max_t_with_range()
{
  // Incluee 3,4
  std::string test_result = to_string(
      *execute_time_aggregation_function_with_range(TS::FunctionId::Maximum, 0, 120, 3, 4));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 3\n"
      "2015-Mar-03 01:00:00 EET -> 4\n"
      "2015-Mar-03 02:00:00 EET -> 4\n"
      "2015-Mar-03 03:00:00 EET -> 4\n"
      "2015-Mar-03 04:00:00 EET -> nan\n";

  if (expected_result != test_result)
    TEST_FAILED("Max-function with range test failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void mean_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Mean, 120, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 1\n"
      "2015-Mar-03 01:00:00 EET -> 1.5\n"
      "2015-Mar-03 02:00:00 EET -> 2\n"
      "2015-Mar-03 03:00:00 EET -> 3\n"
      "2015-Mar-03 04:00:00 EET -> 4\n";

  if (expected_result != test_result)
    TEST_FAILED("Mean-function test failed. Result should be:\n" + expected_result + "\n not \n" +
                test_result);

  TEST_PASSED();
}

void amean_t()
{
  std::string test_result =
      ("mean:\n" + to_string(*execute_time_aggregation_function_with_diverse_values(
                       TS::FunctionId::Mean, 60, 60)));
  test_result.append("amean:\n" + to_string(*execute_time_aggregation_function_with_diverse_values(
                                      TS::FunctionId::Amean, 60, 60)));

  std::string expected_result =
      "mean:\n"
      "2015-Mar-03 00:00:00 EET -> 4\n"
      "2015-Mar-03 01:00:00 EET -> 4.75\n"
      "2015-Mar-03 02:00:00 EET -> 5\n"
      "2015-Mar-03 03:00:00 EET -> 4.25\n"
      "2015-Mar-03 04:00:00 EET -> 4\n"
      "amean:\n"
      "2015-Mar-03 00:00:00 EET -> 4\n"
      "2015-Mar-03 01:00:00 EET -> 4\n"
      "2015-Mar-03 02:00:00 EET -> 5.33333\n"
      "2015-Mar-03 03:00:00 EET -> 4\n"
      "2015-Mar-03 04:00:00 EET -> 4\n";

  if (expected_result != test_result)
    TEST_FAILED("Amean-function test failed. Result should be:\n" + expected_result + "\n not \n" +
                test_result);

  TEST_PASSED();
}

void meandir_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Mean, 120, 0, false, true));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 340\n"
      "2015-Mar-03 01:00:00 EET -> 345\n"
      "2015-Mar-03 02:00:00 EET -> 348.75\n"
      "2015-Mar-03 03:00:00 EET -> 356.25\n"
      "2015-Mar-03 04:00:00 EET -> 3.75\n";

  if (expected_result != test_result)
    TEST_FAILED("MeanDir-function test failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void mean_t_with_range()
{
  // Include 2,3,4
  std::string test_result = to_string(
      *execute_time_aggregation_function_with_range(TS::FunctionId::Mean, 120, 0, 2.0, 4.0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> nan\n"
      "2015-Mar-03 01:00:00 EET -> 2\n"
      "2015-Mar-03 02:00:00 EET -> 2.5\n"
      "2015-Mar-03 03:00:00 EET -> 3\n"
      "2015-Mar-03 04:00:00 EET -> 3.5\n";

  if (expected_result != test_result)
    TEST_FAILED("Mean-function with range test failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void mean_a_t_with_range()
{
  // Include 1,2,3
  std::string test_result = to_string(*execute_time_area_aggregation_function_with_range(
      TS::FunctionId::Mean, TS::FunctionId::Mean, 120, 0, 1.0, 3.0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 1\n"
      "2015-Mar-03 01:00:00 EET -> 1.5\n"
      "2015-Mar-03 02:00:00 EET -> 2\n"
      "2015-Mar-03 03:00:00 EET -> 2.5\n"
      "2015-Mar-03 04:00:00 EET -> 3\n";

  if (expected_result != test_result)
    TEST_FAILED("Mean area and time function with range test failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void mean_t_a_with_range()
{
  // Include 1,2,3
  std::string test_result = to_string(*execute_area_time_aggregation_function_with_range(
      TS::FunctionId::Mean, TS::FunctionId::Mean, 120, 0, 1.0, 3.0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 1\n"
      "2015-Mar-03 01:00:00 EET -> 1.5\n"
      "2015-Mar-03 02:00:00 EET -> 2\n"
      "2015-Mar-03 03:00:00 EET -> nan\n"
      "2015-Mar-03 04:00:00 EET -> nan\n";

  if (expected_result != test_result)
    TEST_FAILED("Mean time and area function with range test failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void median_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Median, 240, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 1\n"
      "2015-Mar-03 01:00:00 EET -> 1.5\n"
      "2015-Mar-03 02:00:00 EET -> 2\n"
      "2015-Mar-03 03:00:00 EET -> 2.5\n"
      "2015-Mar-03 04:00:00 EET -> 3\n";

  if (expected_result != test_result)
    TEST_FAILED("Median-function test failed. Result should be:\n" + expected_result + "\n not \n" +
                test_result);

  TEST_PASSED();
}

void median_t_with_range()
{
  // Include 2,3,4
  std::string test_result = to_string(
      *execute_time_aggregation_function_with_range(TS::FunctionId::Median, 240, 0, 2, 4));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> nan\n"
      "2015-Mar-03 01:00:00 EET -> 2\n"
      "2015-Mar-03 02:00:00 EET -> 2.5\n"
      "2015-Mar-03 03:00:00 EET -> 3\n"
      "2015-Mar-03 04:00:00 EET -> 3\n";

  if (expected_result != test_result)
    TEST_FAILED("Median-function with range test failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void sum_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Sum, 120, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 1\n"
      "2015-Mar-03 01:00:00 EET -> 3\n"
      "2015-Mar-03 02:00:00 EET -> 6\n"
      "2015-Mar-03 03:00:00 EET -> 9\n"
      "2015-Mar-03 04:00:00 EET -> 12\n";

  if (expected_result != test_result)
    TEST_FAILED("Sum-function test failed. Expected result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void integ_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Integ, 120, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 0.000277778\n"
      "2015-Mar-03 01:00:00 EET -> 1.5\n"
      "2015-Mar-03 02:00:00 EET -> 4\n"
      "2015-Mar-03 03:00:00 EET -> 6\n"
      "2015-Mar-03 04:00:00 EET -> 8\n";

  if (expected_result != test_result)
    TEST_FAILED("Integ-function test failed. Expected result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void stddev_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::StandardDeviation, 240, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 0\n"
      "2015-Mar-03 01:00:00 EET -> 0.707107\n"
      "2015-Mar-03 02:00:00 EET -> 0.816497\n"
      "2015-Mar-03 03:00:00 EET -> 1.04881\n"
      "2015-Mar-03 04:00:00 EET -> 1.30931\n";

  if (expected_result != test_result)
    TEST_FAILED("StandardDeviation-function test failed. Expected result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void stddevdir_t()
{
  std::string test_result = to_string(
      *execute_time_aggregation_function(TS::FunctionId::StandardDeviation, 240, 0, false, true));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 0\n"
      "2015-Mar-03 01:00:00 EET -> 7.07107\n"
      "2015-Mar-03 02:00:00 EET -> 6.29153\n"
      "2015-Mar-03 03:00:00 EET -> 8.21584\n"
      "2015-Mar-03 04:00:00 EET -> 9.91031\n";

  if (expected_result != test_result)
    TEST_FAILED("StandardDeviationDir-function test failed. Expected result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void percentage_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Percentage, 120, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 0\n"
      "2015-Mar-03 01:00:00 EET -> 0\n"
      "2015-Mar-03 02:00:00 EET -> 25\n"
      "2015-Mar-03 03:00:00 EET -> 75\n"
      "2015-Mar-03 04:00:00 EET -> 75\n";

  if (expected_result != test_result)
    TEST_FAILED("Percentage-function test failed. Expected result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void count_t()
{
  // not count doesn't make much sense when aggregating over time, since weigths are used
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Count, 120, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 0\n"
      "2015-Mar-03 01:00:00 EET -> 0\n"
      "2015-Mar-03 02:00:00 EET -> 1\n"
      "2015-Mar-03 03:00:00 EET -> 2\n"
      "2015-Mar-03 04:00:00 EET -> 2\n";

  if (expected_result != test_result)
    TEST_FAILED("Count-function test failed. Expected result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void change_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Change, 120, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 0\n"
      "2015-Mar-03 01:00:00 EET -> 1\n"
      "2015-Mar-03 02:00:00 EET -> 2\n"
      "2015-Mar-03 03:00:00 EET -> 2\n"
      "2015-Mar-03 04:00:00 EET -> 2\n";

  if (expected_result != test_result)
    TEST_FAILED("Change-function test failed. Expected result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void trend_t()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Trend, 120, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 32700\n"
      "2015-Mar-03 01:00:00 EET -> 100\n"
      "2015-Mar-03 02:00:00 EET -> 66.6667\n"
      "2015-Mar-03 03:00:00 EET -> 66.6667\n"
      "2015-Mar-03 04:00:00 EET -> 66.6667\n";

  if (expected_result != test_result)
    TEST_FAILED("Trend-function test failed. Expected result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

// area aggregation
void min_a()
{
  std::string test_result = to_string(*execute_area_aggregation_function(TS::FunctionId::Minimum));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 1\n"
      "2015-Mar-03 01:00:00 EET -> 6\n"
      "2015-Mar-03 02:00:00 EET -> 11\n"
      "2015-Mar-03 03:00:00 EET -> 16\n"
      "2015-Mar-03 04:00:00 EET -> 21\n";

  if (test_result != expected_result)
    TEST_FAILED("Minimum-function test for area failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void max_a()
{
  std::string test_result = to_string(*execute_area_aggregation_function(TS::FunctionId::Maximum));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 5\n"
      "2015-Mar-03 01:00:00 EET -> 10\n"
      "2015-Mar-03 02:00:00 EET -> 15\n"
      "2015-Mar-03 03:00:00 EET -> 20\n"
      "2015-Mar-03 04:00:00 EET -> 25\n";

  if (test_result != expected_result)
    TEST_FAILED("Maximum-function test for area failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);
  TEST_PASSED();
}

void max_a_nan()
{
  std::string test_result =
      to_string(*execute_area_aggregation_function_nans(TS::FunctionId::Maximum));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 5\n"
      "2015-Mar-03 01:00:00 EET -> 10\n"
      "2015-Mar-03 02:00:00 EET -> nan\n"
      "2015-Mar-03 03:00:00 EET -> nan\n"
      "2015-Mar-03 04:00:00 EET -> 25\n";

  if (test_result != expected_result)
    TEST_FAILED("Maximum-function test for area failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void max_a_with_range_nan()
{
  // Include values between 5.0 and 15.0
  std::string test_result = to_string(
      *execute_area_aggregation_function_with_range_nans(TS::FunctionId::Maximum, 4.0, 22.0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 5\n"
      "2015-Mar-03 01:00:00 EET -> 10\n"
      "2015-Mar-03 02:00:00 EET -> nan\n"
      "2015-Mar-03 03:00:00 EET -> nan\n"
      "2015-Mar-03 04:00:00 EET -> 22\n";

  if (test_result != expected_result)
    TEST_FAILED("Maximum-function with range test for area failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void mean_a()
{
  std::string test_result = to_string(*execute_area_aggregation_function(TS::FunctionId::Mean));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 3\n"
      "2015-Mar-03 01:00:00 EET -> 8\n"
      "2015-Mar-03 02:00:00 EET -> 13\n"
      "2015-Mar-03 03:00:00 EET -> 18\n"
      "2015-Mar-03 04:00:00 EET -> 23\n";

  if (test_result != expected_result)
    TEST_FAILED("Mean-function test for area failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void mean_a_with_range()
{
  // Inclue values between 3.0 and 15.0
  std::string test_result =
      to_string(*execute_area_aggregation_function_with_range(TS::FunctionId::Mean, 3.0, 15.0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 4\n"
      "2015-Mar-03 01:00:00 EET -> 8\n"
      "2015-Mar-03 02:00:00 EET -> 13\n"
      "2015-Mar-03 03:00:00 EET -> nan\n"
      "2015-Mar-03 04:00:00 EET -> nan\n";

  if (test_result != expected_result)
    TEST_FAILED("Mean-function with_range test for area failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void median_a()
{
  std::string test_result = to_string(*execute_area_aggregation_function(TS::FunctionId::Median));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 3\n"
      "2015-Mar-03 01:00:00 EET -> 8\n"
      "2015-Mar-03 02:00:00 EET -> 13\n"
      "2015-Mar-03 03:00:00 EET -> 18\n"
      "2015-Mar-03 04:00:00 EET -> 23\n";

  if (test_result != expected_result)
    TEST_FAILED("Medin-function test for area failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void sum_a()
{
  std::string test_result = to_string(*execute_area_aggregation_function(TS::FunctionId::Sum));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 15\n"
      "2015-Mar-03 01:00:00 EET -> 40\n"
      "2015-Mar-03 02:00:00 EET -> 65\n"
      "2015-Mar-03 03:00:00 EET -> 90\n"
      "2015-Mar-03 04:00:00 EET -> 115\n";

  if (test_result != expected_result)
    TEST_FAILED("Sum-function test for area failed. Result should be:\n" + expected_result +
                "\n not \n" + test_result);

  TEST_PASSED();
}

void stddev_a()
{
  std::string test_result =
      to_string(*execute_area_aggregation_function(TS::FunctionId::StandardDeviation));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 1.58114\n"
      "2015-Mar-03 01:00:00 EET -> 1.58114\n"
      "2015-Mar-03 02:00:00 EET -> 1.58114\n"
      "2015-Mar-03 03:00:00 EET -> 1.58114\n"
      "2015-Mar-03 04:00:00 EET -> 1.58114\n";

  if (test_result != expected_result)
    TEST_FAILED("StandardDeviation-function test for area failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void percentage_a()
{
  std::string test_result =
      to_string(*execute_area_aggregation_function(TS::FunctionId::Percentage));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 0\n"
      "2015-Mar-03 01:00:00 EET -> 0\n"
      "2015-Mar-03 02:00:00 EET -> 40\n"
      "2015-Mar-03 03:00:00 EET -> 100\n"
      "2015-Mar-03 04:00:00 EET -> 40\n";

  if (expected_result != test_result)
    TEST_FAILED("Percentage-function test for area failed. Expected result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void count_a()
{
  std::string test_result = to_string(*execute_area_aggregation_function(TS::FunctionId::Count));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 0\n"
      "2015-Mar-03 01:00:00 EET -> 0\n"
      "2015-Mar-03 02:00:00 EET -> 2\n"
      "2015-Mar-03 03:00:00 EET -> 5\n"
      "2015-Mar-03 04:00:00 EET -> 2\n";

  if (expected_result != test_result)
    TEST_FAILED("Count-function test for area failed. Expected result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void change_a()
{
  std::string test_result = to_string(*execute_area_aggregation_function(TS::FunctionId::Change));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 4\n"
      "2015-Mar-03 01:00:00 EET -> 4\n"
      "2015-Mar-03 02:00:00 EET -> 4\n"
      "2015-Mar-03 03:00:00 EET -> 4\n"
      "2015-Mar-03 04:00:00 EET -> 4\n";

  if (expected_result != test_result)
    TEST_FAILED("Change-function test for area failed. Expected result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void trend_a()
{
  std::string test_result = to_string(*execute_area_aggregation_function(TS::FunctionId::Trend));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 100\n"
      "2015-Mar-03 01:00:00 EET -> 100\n"
      "2015-Mar-03 02:00:00 EET -> 100\n"
      "2015-Mar-03 03:00:00 EET -> 100\n"
      "2015-Mar-03 04:00:00 EET -> 100\n";

  if (expected_result != test_result)
    TEST_FAILED("Trend-function test for area failed. Expected result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

// first time then area aggregation
void min_max_ta()
{
  std::string test_result = to_string(*execute_time_area_aggregation_function(
      TS::FunctionId::Minimum, TS::FunctionId::Maximum, 120, 0));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 5\n"
      "2015-Mar-03 01:00:00 EET -> 5\n"
      "2015-Mar-03 02:00:00 EET -> 5\n"
      "2015-Mar-03 03:00:00 EET -> 10\n"
      "2015-Mar-03 04:00:00 EET -> 15\n";

  if (test_result != expected_result)
    TEST_FAILED("Minimum-Maximum-function test for time and area failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

// first area then time aggregation
void min_max_at()
{
  std::string test_result = to_string(*execute_area_time_aggregation_function(
      TS::FunctionId::Minimum, TS::FunctionId::Maximum, 0, 120));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 11\n"
      "2015-Mar-03 01:00:00 EET -> 16\n"
      "2015-Mar-03 02:00:00 EET -> 21\n"
      "2015-Mar-03 03:00:00 EET -> 21\n"
      "2015-Mar-03 04:00:00 EET -> 21\n";

  if (test_result != expected_result)
    TEST_FAILED("Minimum-Maximum-function test for area and time failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

void min_t_missing_value()
{
  std::string test_result =
      to_string(*execute_time_aggregation_function(TS::FunctionId::Minimum, 120, 0, true));

  std::string expected_result =
      "2015-Mar-03 00:00:00 EET -> 1\n"
      "2015-Mar-03 01:00:00 EET -> 1\n"
      "2015-Mar-03 02:00:00 EET -> 1\n"
      "2015-Mar-03 03:00:00 EET -> nan\n"
      "2015-Mar-03 04:00:00 EET -> nan\n";

  if (test_result != expected_result)
    TEST_FAILED("Minimum-function with missing value test failed. Result should be:\n" +
                expected_result + "\n not \n" + test_result);

  TEST_PASSED();
}

// ----------------------------------------------------------------------
/*!
 * The actual test suite
 */
// ----------------------------------------------------------------------

class tests : public tframe::tests
{
  virtual const char* error_message_prefix() const { return "\n\t"; }
  void test()
  {
    // time aggregation
    TEST(nearest_t);
    TEST(interpolate_t);
    TEST(min_t);
    TEST(max_t);
    TEST(mean_t);
    TEST(amean_t);
    TEST(median_t);
    TEST(sum_t);
    TEST(integ_t);
    TEST(stddev_t);
    TEST(percentage_t);
    TEST(count_t);
    TEST(change_t);
    TEST(trend_t);
    // area aggregation
    TEST(min_a);
    TEST(max_a);
    TEST(max_a_nan);
    TEST(mean_a);
    TEST(median_a);
    TEST(sum_a);
    TEST(stddev_a);
    TEST(percentage_a);
    TEST(count_a);
    TEST(change_a);
    TEST(trend_a);

    // modulo aggregation
    TEST(interpolatedir_t);
    TEST(meandir_t);
    TEST(stddevdir_t);

    // first time then area aggregation
    TEST(min_max_ta);
    // first area then time aggregation
    TEST(min_max_at);
    // missing value in time series
    TEST(min_t_missing_value);
    // tests functions with inclusive ranges
    TEST(min_t_with_range);
    TEST(max_t_with_range);
    TEST(mean_t_with_range);
    TEST(median_t_with_range);
    TEST(mean_a_with_range);
    TEST(max_a_with_range_nan);
    TEST(mean_a_t_with_range);
    TEST(mean_t_a_with_range);
  }
};

}  // namespace TimeSeriesAggregatorTest

//! The main program
int main()
{
  using namespace std;
  cout << endl << "TimeSeriesAggregator tester" << endl << "==========================" << endl;
  TimeSeriesAggregatorTest::tests t;
  return t.run();
}
