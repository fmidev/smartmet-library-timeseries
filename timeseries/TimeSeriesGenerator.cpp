// ======================================================================
/*!
 * \brief Time series generator for forecasts and observations
 */
// ======================================================================

#include "TimeSeriesGenerator.h"
#include <macgyver/Exception.h>
#include <macgyver/TimeParser.h>

namespace SmartMet
{
namespace TimeSeries
{
namespace TimeSeriesGenerator
{
namespace
{
const int default_timestep = 60;

// Hard safety limit on how many iterations a single time-series generation may perform,
// independent of the configurable RequestLimits::maxtimes (which is only a post-generation
// policy check). Without it, an unbounded endtime, a huge timesteps count, or a days/time
// filter that excludes everything drives the generation loops into a multi-billion
// iteration build that OOM-kills or hangs the process. The limit is generous (10M steps is
// ~19 years of one-minute data), so it never rejects a realistic query; it only stops abuse.
const std::size_t max_generated_timesteps = 10ULL * 1000 * 1000;

void check_generation_limit(std::size_t iterations)
{
  if (iterations > max_generated_timesteps)
    throw Fmi::Exception(BCP, "Too many time steps requested")
        .addParameter("maximum", std::to_string(max_generated_timesteps));
}

// The effective limit on the number of generated times: the caller's limit (0 = none) capped
// by the hard safety limit.
std::size_t effective_limit(std::size_t theMaxTimes)
{
  if (theMaxTimes == 0 || theMaxTimes > max_generated_timesteps)
    return max_generated_timesteps;
  return theMaxTimes;
}

// Insert a generated time and stop if the output grows beyond the limit. The iteration
// counters above do not bound the output size: in fixed-times mode one iteration is a day,
// which may insert up to 1440 times.
void insert_time(std::set<Fmi::LocalDateTime>& theTimes,
                 const Fmi::LocalDateTime& t,
                 std::size_t theLimit)
{
  theTimes.insert(t);
  if (theTimes.size() > theLimit)
    throw Fmi::Exception(BCP, "Too many time steps requested")
        .addParameter("maximum", std::to_string(theLimit));
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate fixed HHMM times
 */
// ----------------------------------------------------------------------

void generate_fixedtimes_until_endtime(std::set<Fmi::LocalDateTime>& theTimes,
                                       const TimeSeriesGeneratorOptions& theOptions,
                                       const Fmi::LocalDateTime& theStartTime,
                                       const Fmi::LocalDateTime& theEndTime,
                                       const Fmi::TimeZonePtr& theZone,
                                       std::size_t theLimit)
{
  try
  {
    Fmi::LocalTimePeriod period(theStartTime, theEndTime);

    Fmi::Date day(theStartTime.local_time().date());

    std::size_t iterations = 0;
    while (true)
    {
      check_generation_limit(++iterations);
      for (unsigned int hhmm : theOptions.timeList)
      {
        unsigned int hh = hhmm / 100;
        unsigned int mm = hhmm % 100;

        Fmi::LocalDateTime d =
            Fmi::TimeParser::make_time(day, Fmi::Hours(hh) + Fmi::Minutes(mm), theZone);

        if (d.is_not_a_date_time())
          continue;

        if (!theOptions.days.empty())
          if (theOptions.days.find(day.day()) == theOptions.days.end())
            continue;

        if (period.contains(d) || d == theEndTime)
          insert_time(theTimes, d, theLimit);
        if (day > theEndTime.local_time().date())
          return;
      }

      ++day;
    }
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate fixed HHMM times
 */
// ----------------------------------------------------------------------

void generate_fixedtimes_for_number_of_steps(std::set<Fmi::LocalDateTime>& theTimes,
                                             const TimeSeriesGeneratorOptions& theOptions,
                                             const Fmi::LocalDateTime& theStartTime,
                                             const Fmi::LocalDateTime& theEndTime,
                                             const Fmi::TimeZonePtr& theZone,
                                             std::size_t theLimit)
{
  try
  {
    Fmi::LocalTimePeriod period(theStartTime, theEndTime);

    Fmi::Date day(theStartTime.local_time().date());

    std::size_t iterations = 0;
    while (true)
    {
      check_generation_limit(++iterations);
      for (unsigned int hhmm : theOptions.timeList)
      {
        unsigned int hh = hhmm / 100;
        unsigned int mm = hhmm % 100;

        Fmi::LocalDateTime d =
            Fmi::TimeParser::make_time(day, Fmi::Hours(hh) + Fmi::Minutes(mm), theZone);

        if (d.is_not_a_date_time())
          continue;

        if (!theOptions.days.empty())
          if (theOptions.days.find(day.day()) == theOptions.days.end())
            continue;

        if (d >= theStartTime)
          insert_time(theTimes, d, theLimit);
        if (theTimes.size() >= *theOptions.timeSteps)
          return;
      }

      ++day;
    }
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate fixed HHMM times
 */
// ----------------------------------------------------------------------

void generate_fixedtimes(std::set<Fmi::LocalDateTime>& theTimes,
                         const TimeSeriesGeneratorOptions& theOptions,
                         const Fmi::LocalDateTime& theStartTime,
                         const Fmi::LocalDateTime& theEndTime,
                         const Fmi::TimeZonePtr& theZone,
                         std::size_t theLimit)
{
  try
  {
    if (theOptions.timeList.empty())
      return;

    if (!theOptions.timeSteps)
      generate_fixedtimes_until_endtime(
          theTimes, theOptions, theStartTime, theEndTime, theZone, theLimit);
    else
      generate_fixedtimes_for_number_of_steps(
          theTimes, theOptions, theStartTime, theEndTime, theZone, theLimit);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Insert a timestep if valid, return true if the step limit was reached.
 */
// ----------------------------------------------------------------------

bool insert_timestep_if_valid(std::set<Fmi::LocalDateTime>& theTimes,
                              const TimeSeriesGeneratorOptions& theOptions,
                              const Fmi::LocalDateTime& t,
                              const Fmi::LocalTimePeriod& period,
                              const Fmi::LocalDateTime& theStartTime,
                              const Fmi::LocalDateTime& theEndTime,
                              std::size_t theLimit)
{
  if (!!theOptions.timeSteps)
  {
    if (!t.is_not_a_date_time() && t >= theStartTime)
      insert_time(theTimes, t, theLimit);
    return theTimes.size() >= *theOptions.timeSteps;
  }
  if (!t.is_not_a_date_time() && (period.contains(t) || t == theEndTime))
    insert_time(theTimes, t, theLimit);
  return false;
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate time step series
 *
 * There may be a fixed number of timesteps or a fixed end time.
 */
// ----------------------------------------------------------------------

void generate_timesteps(std::set<Fmi::LocalDateTime>& theTimes,
                        const TimeSeriesGeneratorOptions& theOptions,
                        const Fmi::LocalDateTime& theStartTime,
                        const Fmi::LocalDateTime& theEndTime,
                        const Fmi::TimeZonePtr& theZone,
                        std::size_t theLimit)
{
  try
  {
    unsigned int timestep = (!theOptions.timeStep ? default_timestep : *theOptions.timeStep);

    // Special case timestep=0: return {starttime,endtime}, or a single element list if they're the
    // same
    if (timestep == 0)
    {
      theTimes.insert(theStartTime);
      theTimes.insert(theEndTime);
      return;
    }

    // Normal case: timeStep > 0

    Fmi::LocalTimePeriod period(theStartTime, theEndTime);
    Fmi::Date day(theStartTime.local_time().date());

    int mins = 0;

    std::size_t iterations = 0;
    while (true)
    {
      check_generation_limit(++iterations);
      if (mins >= 24 * 60)
      {
        mins -= 24 * 60;
        day++;
      }

      Fmi::LocalDateTime t = Fmi::TimeParser::make_time(day, Fmi::Minutes(mins), theZone);

      if (t > theEndTime)
        break;

      if (!theOptions.days.empty() && theOptions.days.find(day.day()) == theOptions.days.end())
      {
        mins += timestep;
        continue;
      }

      // In the first case we can test after the insert if
      // we should break based on the number of times, in
      // the latter case we must validate the time first
      // to prevent its inclusion.

      if (insert_timestep_if_valid(
              theTimes, theOptions, t, period, theStartTime, theEndTime, theLimit))
        break;

      mins += timestep;
    }
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate data steps
 */
// ----------------------------------------------------------------------

void generate_datatimes_climatology(std::set<Fmi::LocalDateTime>& theTimes,
                                    const TimeSeriesGeneratorOptions& theOptions,
                                    const Fmi::LocalDateTime& theStartTime,
                                    const Fmi::LocalDateTime& theEndTime,
                                    const Fmi::TimeZonePtr& theZone)
{
  try
  {
    Fmi::LocalTimePeriod period(theStartTime, theEndTime);

    // Climatology - must change years accordingly
    short unsigned int startyear = theStartTime.date().year();
    short unsigned int endyear = theEndTime.date().year();

    // Note that startyear<endyear is possible, hence we need to loop over possible years
    for (short unsigned int year = startyear; year <= endyear; ++year)
    {
      // Handle all climatology times
      for (const Fmi::DateTime& t : *theOptions.getDataTimes())
      {
        // Done if a max number of timesteps was requested
        if (theOptions.timeSteps && theTimes.size() >= *theOptions.timeSteps)
          break;

        try
        {
          Fmi::DateTime t2(Fmi::Date(year, t.date().month(), t.date().day()), t.time_of_day());
          Fmi::LocalDateTime lt(t2, theZone);
          // Done if beyond the requested end time
          if (lt > theEndTime)
            break;

          if (!theOptions.days.empty())
            if (theOptions.days.find(lt.date().day()) == theOptions.days.end())
              continue;

          if (period.contains(lt) || lt == theEndTime)
            theTimes.insert(lt);
        }
        catch (...)
        {
          // 29.2. does not exist for all years
        }
      }
    }
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Check whether a local time passes the day-of-month filter.
 */
// ----------------------------------------------------------------------

bool passes_day_filter(const Fmi::LocalDateTime& lt, const std::set<unsigned int>& days)
{
  return days.empty() || days.find(lt.date().day()) != days.end();
}

// ----------------------------------------------------------------------
/*!
 * \brief Collect data times up to a maximum count.
 */
// ----------------------------------------------------------------------

void collect_datatimes_with_limit(std::set<Fmi::LocalDateTime>& theTimes,
                                  const TimeSeriesGeneratorOptions& theOptions,
                                  const Fmi::LocalDateTime& theStartTime,
                                  const Fmi::TimeZonePtr& theZone)
{
  for (const Fmi::DateTime& t : *theOptions.getDataTimes())
  {
    Fmi::LocalDateTime lt(t, theZone);
    if (!passes_day_filter(lt, theOptions.days))
      continue;
    if (lt >= theStartTime && theTimes.size() < *theOptions.timeSteps)
      theTimes.insert(lt);
    if (theTimes.size() >= *theOptions.timeSteps)
      break;
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Collect data times within a time period.
 */
// ----------------------------------------------------------------------

void collect_datatimes_in_period(std::set<Fmi::LocalDateTime>& theTimes,
                                 const TimeSeriesGeneratorOptions& theOptions,
                                 const Fmi::LocalTimePeriod& period,
                                 const Fmi::LocalDateTime& theEndTime,
                                 const Fmi::TimeZonePtr& theZone)
{
  for (const Fmi::DateTime& t : *theOptions.getDataTimes())
  {
    Fmi::LocalDateTime lt(t, theZone);
    if (!passes_day_filter(lt, theOptions.days))
      continue;
    if (period.contains(lt) || lt == theEndTime)
      theTimes.insert(lt);
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate data steps
 */
// ----------------------------------------------------------------------

void generate_datatimes_normal(std::set<Fmi::LocalDateTime>& theTimes,
                               const TimeSeriesGeneratorOptions& theOptions,
                               const Fmi::LocalDateTime& theStartTime,
                               const Fmi::LocalDateTime& theEndTime,
                               const Fmi::TimeZonePtr& theZone)
{
  try
  {
    Fmi::LocalTimePeriod period(theStartTime, theEndTime);

    // The timesteps available in the data itself - Normal data, no need to fiddle with years
    if (!!theOptions.timeSteps)
      collect_datatimes_with_limit(theTimes, theOptions, theStartTime, theZone);
    else
      collect_datatimes_in_period(theTimes, theOptions, period, theEndTime, theZone);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate data steps
 */
// ----------------------------------------------------------------------

void generate_datatimes(std::set<Fmi::LocalDateTime>& theTimes,
                        const TimeSeriesGeneratorOptions& theOptions,
                        const Fmi::LocalDateTime& theStartTime,
                        const Fmi::LocalDateTime& theEndTime,
                        const Fmi::TimeZonePtr& theZone)
{
  try
  {
    if (theOptions.isClimatology)
      generate_datatimes_climatology(theTimes, theOptions, theStartTime, theEndTime, theZone);
    else
      generate_datatimes_normal(theTimes, theOptions, theStartTime, theEndTime, theZone);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate data steps for graphs
 */
// ----------------------------------------------------------------------

void generate_graphtimes(std::set<Fmi::LocalDateTime>& theTimes,
                         const TimeSeriesGeneratorOptions& theOptions,
                         const Fmi::LocalDateTime& theStartTime,
                         const Fmi::LocalDateTime& theEndTime,
                         const Fmi::TimeZonePtr& theZone)
{
  try
  {
    // Wall clock time rounded up to the next hour so that
    // the graph doesn't have a gap at the beginning

    // Round the start up to the next hour

    long extraseconds = theStartTime.local_time().time_of_day().total_seconds() % 3600;
    if (extraseconds > 0)
    {
      Fmi::LocalDateTime t1(theStartTime);
      t1 += Fmi::Seconds(3600 - extraseconds);
      theTimes.insert(t1);
    }

    generate_datatimes(theTimes, theOptions, theStartTime, theEndTime, theZone);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // namespace

// ----------------------------------------------------------------------
/*!
 * \brief Generate time series for the given timezone
 */
// ----------------------------------------------------------------------

LocalTimeList generate(const TimeSeriesGeneratorOptions& theOptions,
                       const Fmi::TimeZonePtr& theZone)
{
  return generate(theOptions, theZone, 0);
}

// ----------------------------------------------------------------------
/*!
 * \brief Generate time series for the given timezone with a limit on the number of times
 *
 * Generation fails if the fixed-times or timestep modes would produce more than
 * min(theMaxTimes, hard limit) times, where theMaxTimes=0 means only the hard limit.
 * Data times are bounded by the data itself and are not limited.
 */
// ----------------------------------------------------------------------

LocalTimeList generate(const TimeSeriesGeneratorOptions& theOptions,
                       const Fmi::TimeZonePtr& theZone,
                       std::size_t theMaxTimes)
{
  try
  {
    const std::size_t limit = effective_limit(theMaxTimes);

    // Determine start and end times

    Fmi::LocalDateTime starttime(Fmi::LocalDateTime::NOT_A_DATE_TIME);
    Fmi::LocalDateTime endtime(Fmi::LocalDateTime::NOT_A_DATE_TIME);

    // Adjust to given timezone if input was not UTC. Note that if start and end times
    // are omitted, we use the data times for climatology data just like for normal data.

    if (theOptions.startTimeData)
    {
      if (theOptions.getDataTimes()->empty())
        return {};
      starttime = Fmi::LocalDateTime(theOptions.getDataTimes()->front(), theZone);
    }
    else if (!theOptions.startTimeUTC)
      starttime = Fmi::TimeParser::make_time(
          theOptions.startTime.date(), theOptions.startTime.time_of_day(), theZone);
    else
      starttime = Fmi::LocalDateTime(theOptions.startTime, theZone);

    if (theOptions.endTimeData)
    {
      if (theOptions.getDataTimes()->empty())
        return {};
      endtime = Fmi::LocalDateTime(theOptions.getDataTimes()->back(), theZone);
    }
    else if (!theOptions.endTimeUTC)
      endtime = Fmi::TimeParser::make_time(
          theOptions.endTime.date(), theOptions.endTime.time_of_day(), theZone);
    else
      endtime = Fmi::LocalDateTime(theOptions.endTime, theZone);

    // Start generating a set of unique local times

    std::set<Fmi::LocalDateTime> times;

    switch (theOptions.mode)
    {
      case TimeSeriesGeneratorOptions::FixedTimes:
        generate_fixedtimes(times, theOptions, starttime, endtime, theZone, limit);
        break;
      case TimeSeriesGeneratorOptions::TimeSteps:
        generate_timesteps(times, theOptions, starttime, endtime, theZone, limit);
        break;
      case TimeSeriesGeneratorOptions::DataTimes:
        generate_datatimes(times, theOptions, starttime, endtime, theZone);
        break;
      case TimeSeriesGeneratorOptions::GraphTimes:
        generate_graphtimes(times, theOptions, starttime, endtime, theZone);
        break;
    }

    // Copy the unique times into actual output container

    LocalTimeList ret;
    std::copy(times.begin(), times.end(), back_inserter(ret));
    return ret;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // namespace TimeSeriesGenerator
}  // namespace TimeSeries
}  // namespace SmartMet
