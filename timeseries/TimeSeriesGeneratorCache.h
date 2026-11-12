// ======================================================================
/*!
 * \brief Cache for generated timeseries
 */
// ======================================================================

#pragma once

#include "TimeSeriesGenerator.h"
#include "TimeSeriesGeneratorOptions.h"
#include <macgyver/Cache.h>
#include <macgyver/CacheStats.h>
#include <macgyver/LocalDateTime.h>
#include <cstddef>
#include <memory>

namespace SmartMet
{
namespace TimeSeries
{
class TimeSeriesGeneratorCache
{
 public:
  using TimeList = std::shared_ptr<TimeSeriesGenerator::LocalTimeList>;

  void resize(std::size_t theSize) const;

  TimeList generate(const TimeSeriesGeneratorOptions& theOptions,
                    const Fmi::TimeZonePtr& theZone) const;

  // As above, but limit the number of generated times (0 = no limit), see
  // TimeSeriesGenerator::generate
  TimeList generate(const TimeSeriesGeneratorOptions& theOptions,
                    const Fmi::TimeZonePtr& theZone,
                    std::size_t theMaxTimes) const;

  Fmi::Cache::CacheStats getCacheStats() const { return itsCache.statistics(); }

 private:
  mutable Fmi::Cache::Cache<std::size_t, TimeList> itsCache;
};

}  // namespace TimeSeries
}  // namespace SmartMet
