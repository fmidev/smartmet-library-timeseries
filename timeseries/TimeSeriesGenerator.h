// ======================================================================
/*!
 * \brief Time series generator for forecasts and observations
 */
// ======================================================================

#pragma once

#include "TimeSeriesGeneratorOptions.h"

#include <macgyver/LocalDateTime.h>
#include <list>
#include <string>

namespace SmartMet
{
namespace TimeSeries
{
namespace TimeSeriesGenerator
{
using LocalTimeList = std::list<Fmi::LocalDateTime>;

LocalTimeList generate(const TimeSeriesGeneratorOptions& theOptions,
                       const Fmi::TimeZonePtr& theZone);

}  // namespace TimeSeriesGenerator
}  // namespace TimeSeries
}  // namespace SmartMet
