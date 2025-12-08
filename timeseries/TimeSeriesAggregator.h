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
TimeSeriesPtr aggregate(const TimeSeries& ts,
                        const DataFunctions& pf,
                        const TimeSeriesGenerator::LocalTimeList& timesteps);

TimeSeriesGroupPtr aggregate(const TimeSeriesGroup& ts_group,
                             const DataFunctions& pf,
                             const TimeSeriesGenerator::LocalTimeList& timesteps);

TimedValue time_aggregate(const TimeSeries& ts,
                          const DataFunction& func,
                          const Fmi::LocalDateTime& timestep);

/**
 * @brief Aggregate time series data (selected tiemsteps only)
 *
 * @param ts Time series data. It is assumed that the data is sorted by time.
 * @param func Data function
 * @param timesteps List of timesteps for which to aggregate data. It is assumed
 *                  that the list is sorted by time. Empty list means that the
 *                  result will be also empty.
 */
TimeSeriesPtr time_aggregate(const TimeSeries& ts,
                             const DataFunction& func,
                             const TimeSeriesGenerator::LocalTimeList& timesteps);

}  // namespace Aggregator
}  // namespace TimeSeries
}  // namespace SmartMet

// ======================================================================
