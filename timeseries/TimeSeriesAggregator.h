// ======================================================================
/*!
 * \brief Interface of class ValueAggregator
 */
// ======================================================================

#pragma once

#include "DataFunction.h"
#include "TimeSeries.h"
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

}  // namespace Aggregator
}  // namespace TimeSeries
}  // namespace SmartMet

// ======================================================================
