// ======================================================================
/*!
 * \brief Time series generator for forecasts and observations
 */
// ======================================================================

#pragma once

#include "TimeSeriesGeneratorOptions.h"
#include "TimeSeriesTypes.h"
#include <macgyver/LocalDateTime.h>
#include <cstddef>

namespace SmartMet
{
namespace TimeSeries
{
namespace TimeSeriesGenerator
{
using LocalTimeList = ::SmartMet::TimeSeries::LocalTimeList;

LocalTimeList generate(const TimeSeriesGeneratorOptions& theOptions,
                       const Fmi::TimeZonePtr& theZone);

// As above, but fail if the fixed-times or timestep modes would generate more than
// theMaxTimes times (0 = no limit). A hard safety limit always applies.
LocalTimeList generate(const TimeSeriesGeneratorOptions& theOptions,
                       const Fmi::TimeZonePtr& theZone,
                       std::size_t theMaxTimes);

}  // namespace TimeSeriesGenerator
}  // namespace TimeSeries
}  // namespace SmartMet
