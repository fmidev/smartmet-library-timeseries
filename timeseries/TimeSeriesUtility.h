#pragma once

#include "TimeSeries.h"
#include "TimeSeriesAggregator.h"
#include "TimeSeriesGeneratorCache.h"
#include <macgyver/Exception.h>

namespace SmartMet
{
namespace TimeSeries
{
/*** typedefs ***/
using TimeSeriesData = std::variant<TimeSeriesPtr, TimeSeriesVectorPtr, TimeSeriesGroupPtr>;

using OutputData = std::vector<std::pair<std::string, std::vector<TimeSeriesData> > >;
using PressureLevelParameterPair = std::pair<int, std::string>;
using ParameterTimeSeriesMap = std::map<PressureLevelParameterPair, TimeSeriesPtr>;
using ParameterTimeSeriesGroupMap = std::map<PressureLevelParameterPair, TimeSeriesGroupPtr>;
using FmisidTSVectorPair = std::pair<int, TimeSeriesVectorPtr>;
using TimeSeriesByLocation = std::vector<FmisidTSVectorPair>;
/*** functions ***/
TimeSeriesPtr erase_redundant_timesteps(TimeSeriesPtr ts,
                                        const TimeSeriesGenerator::LocalTimeList& timesteps);
TimeSeriesVectorPtr erase_redundant_timesteps(TimeSeriesVectorPtr tsv,
                                              const TimeSeriesGenerator::LocalTimeList& timesteps);
TimeSeriesGroupPtr erase_redundant_timesteps(TimeSeriesGroupPtr tsg,
                                             const TimeSeriesGenerator::LocalTimeList& timesteps);
size_t number_of_elements(const OutputData& outputData);
TimeSeriesByLocation get_timeseries_by_fmisid(const std::string& producer,
											  const TimeSeriesVectorPtr& observation_result,
											  const TimeSeriesGeneratorCache::TimeList& tlist,
											  int fmisid_index);
int get_fmisid_value(const TimeSeries& ts);

std::ostream& operator<<(std::ostream& os, const TimeSeriesData& tsdata);
std::ostream& operator<<(std::ostream& os, const OutputData& odata);

template <typename T>
T aggregate(const T& raw_data, const DataFunctions& pf, const TimeSeriesGenerator::LocalTimeList& timesteps)
{
  try
  {
    T aggregated_data = Aggregator::aggregate(*raw_data, pf, timesteps);

    return aggregated_data;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // namespace TimeSeries
}  // namespace SmartMet
