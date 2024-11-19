// ======================================================================
/*!
 * \brief Interface of class ValueAggregator
 */
// ======================================================================

#pragma once

#include "DataFunction.h"
#include "TimeSeries.h"
#include "TimeSeriesGenerator.h"
#include <macgyver/Exception.h>

#include <stdexcept>

namespace SmartMet
{
namespace TimeSeries
{
namespace Aggregator
{
TimeSeriesPtr aggregate(const TimeSeries& ts, const DataFunctions& pf);
TimeSeriesGroupPtr aggregate(const TimeSeriesGroup& ts_group, const DataFunctions& pf);
TimedValue time_aggregate(const TimeSeries& ts,
                          const DataFunction& func,
                          const Fmi::LocalDateTime& timestep);

/**
 * @brief Aggregate time series data
 *
 * @param ts Time series data. It is assumed that the data is sorted by time.
 * @param func Data function
 * @param timesteps List of timesteps for which to aggregate data. Empty list means all timesteps. It is assumed
 *                  that the list is sorted by time.
 */
TimeSeriesPtr time_aggregate(
        const TimeSeries& ts,
        const DataFunction& func,
        const TimeSeriesGenerator::LocalTimeList& timesteps);

}  // namespace Aggregator
}  // namespace TimeSeries
}  // namespace SmartMet

// ======================================================================
