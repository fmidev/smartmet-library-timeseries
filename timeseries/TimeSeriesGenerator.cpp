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

// ----------------------------------------------------------------------
/*!
 * \brief Generate fixed HHMM times
 */
// ----------------------------------------------------------------------

void generate_fixedtimes_until_endtime(std::set<Fmi::LocalDateTime>& theTimes,
                                       const TimeSeriesGeneratorOptions& theOptions,
                                       const Fmi::LocalDateTime& theStartTime,
                                       const Fmi::LocalDateTime& theEndTime,
                                       const Fmi::TimeZonePtr& theZone)
{
  try
  {
    Fmi::LocalTimePeriod period(theStartTime, theEndTime);

    Fmi::Date day(theStartTime.local_time().date());

    while (true)
    {
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
          theTimes.insert(d);
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
                                             const Fmi::TimeZonePtr& theZone)
{
  try
  {
    Fmi::LocalTimePeriod period(theStartTime, theEndTime);

    Fmi::Date day(theStartTime.local_time().date());

    while (true)
    {
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
          theTimes.insert(d);
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
                         const Fmi::TimeZonePtr& theZone)
{
  try
  {
    if (theOptions.timeList.empty())
      return;

    if (!theOptions.timeSteps)
      generate_fixedtimes_until_endtime(theTimes, theOptions, theStartTime, theEndTime, theZone);
    else
      generate_fixedtimes_for_number_of_steps(
          theTimes, theOptions, theStartTime, theEndTime, theZone);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
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
                        const Fmi::TimeZonePtr& theZone)
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

    while (true)
    {
      if (mins >= 24 * 60)
      {
        mins -= 24 * 60;
        day++;
      }

      Fmi::LocalDateTime t = Fmi::TimeParser::make_time(day, Fmi::Minutes(mins), theZone);

      if (t > theEndTime)
        break;

      if (!theOptions.days.empty())
        if (theOptions.days.find(day.day()) == theOptions.days.end())
        {
          mins += timestep;
          continue;
        }

      // In the first case we can test after the insert if
      // we should break based on the number of times, in
      // the latter case we must validate the time first
      // to prevent its inclusion.

      if (!!theOptions.timeSteps)
      {
        if (!t.is_not_a_date_time() && t >= theStartTime)
          theTimes.insert(t);

        if (theTimes.size() >= *theOptions.timeSteps)
          break;
      }
      else
      {
        if (!t.is_not_a_date_time() && (period.contains(t) || t == theEndTime))
          theTimes.insert(t);
      }

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

    // The timesteps available in the data itself

    bool use_timesteps = !!theOptions.timeSteps;

    // Normal data - no need to fiddle with years
    if (use_timesteps)
    {
      for (const Fmi::DateTime& t : *theOptions.getDataTimes())
      {
        Fmi::LocalDateTime lt(t, theZone);

        if (!theOptions.days.empty())
          if (theOptions.days.find(lt.date().day()) == theOptions.days.end())
            continue;

        if (lt >= theStartTime && theTimes.size() < *theOptions.timeSteps)
          theTimes.insert(lt);
        if (theTimes.size() >= *theOptions.timeSteps)
          break;
      }
    }
    else
    {
      for (const Fmi::DateTime& t : *theOptions.getDataTimes())
      {
        Fmi::LocalDateTime lt(t, theZone);

        if (!theOptions.days.empty())
          if (theOptions.days.find(lt.date().day()) == theOptions.days.end())
            continue;

        if (period.contains(lt) || lt == theEndTime)
          theTimes.insert(lt);
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
  try
  {
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
        generate_fixedtimes(times, theOptions, starttime, endtime, theZone);
        break;
      case TimeSeriesGeneratorOptions::TimeSteps:
        generate_timesteps(times, theOptions, starttime, endtime, theZone);
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
