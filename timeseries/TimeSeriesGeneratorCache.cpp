#include "TimeSeriesGeneratorCache.h"
#include <macgyver/Exception.h>
#include <macgyver/Hash.h>
#include <list>
#include <optional>
#include <string>

namespace SmartMet
{
namespace TimeSeries
{
namespace
{
// Longer lists are not cached to bound the memory used by the cache, whose size is configured
// as a number of lists. An element takes about 64 bytes, so a list takes at most ~640 kB and
// the default 10000 lists at most ~6.4 GB even if requests vary the start time to fill the
// cache. A typical list has at most a few hundred elements, and a non-cached list is simply
// regenerated.
const std::size_t max_cached_list_size = 10000;
}  // namespace

// ----------------------------------------------------------------------
/*!
 * \brief Resize the cache
 *
 * The user is expected to resize the cache to a user configurable
 * value during startup.
 */
// ----------------------------------------------------------------------

void TimeSeriesGeneratorCache::resize(std::size_t theSize) const
{
  try
  {
    itsCache.resize(theSize);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate the time series
 *
 * Use cache if possible, otherwise generate and cache
 */
// ----------------------------------------------------------------------

TimeSeriesGeneratorCache::TimeList TimeSeriesGeneratorCache::generate(
    const TimeSeriesGeneratorOptions& theOptions, const Fmi::TimeZonePtr& theZone) const
{
  return generate(theOptions, theZone, 0);
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate the time series with a limit on the number of generated times
 *
 * Very long lists are not cached so that the cache memory use stays bounded.
 */
// ----------------------------------------------------------------------

TimeSeriesGeneratorCache::TimeList TimeSeriesGeneratorCache::generate(
    const TimeSeriesGeneratorOptions& theOptions,
    const Fmi::TimeZonePtr& theZone,
    std::size_t theMaxTimes) const
{
  try
  {
    // hash value for the query
    std::size_t hash = theOptions.hash_value();
    Fmi::hash_combine(hash, Fmi::hash_value(theZone));

    // use cached result if possible
    auto cached_result = itsCache.find(hash);
    if (cached_result)
    {
      // The cached list may have been generated with a larger limit. Data times are not limited.
      const auto& series = *cached_result;
      if (theMaxTimes > 0 && series->size() > theMaxTimes &&
          (theOptions.mode == TimeSeriesGeneratorOptions::FixedTimes ||
           theOptions.mode == TimeSeriesGeneratorOptions::TimeSteps))
        throw Fmi::Exception(BCP, "Too many time steps requested")
            .addParameter("maximum", std::to_string(theMaxTimes));
      return series;
    }

    // generate time series and cache it for future use
    TimeList series(new TimeSeriesGenerator::LocalTimeList(
        TimeSeriesGenerator::generate(theOptions, theZone, theMaxTimes)));
    if (series->size() <= max_cached_list_size)
      itsCache.insert(hash, series);
    return series;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // namespace TimeSeries
}  // namespace SmartMet
