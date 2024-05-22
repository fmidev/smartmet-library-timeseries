// ======================================================================
/*!
 * \brief Options for generating a time series
 */
// ======================================================================

#pragma once

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <macgyver/DateTime.h>

#include <list>
#include <ostream>
#include <set>

#include <spine/HTTP.h>

namespace SmartMet
{
namespace TimeSeries
{
struct TimeSeriesGeneratorOptions
{
 public:
  enum Mode
  {
    DataTimes,   // timesteps in querydata
    GraphTimes,  // above plus "now"
    FixedTimes,  // fixed Fmi::SecondClock etc
    TimeSteps    // fixed timestep
  };

  struct Defaults
  {
    /**
     * Default start offset in minutes from the current time.
     *
     * Used to calculate the start time if it is not provided in the request and cannot be
     * determined from other request parameters
    */
    int defaultStartOffset;

    /**
     * Default length in minutes for the time series.
     *
     * Used to calculate the end time if it is not provided in the request and cannot be
     * determined from other request parameters
    */
    unsigned defaultIntervalLength;

    /**
     * Default timestep in minutes.
    */
    unsigned defaultTimeStep;

    constexpr Defaults()
      : defaultStartOffset(0)
      , defaultIntervalLength(1440)
      , defaultTimeStep(60)
    {
    }
  };

  // Timesteps established from the outside
  using TimeList = boost::shared_ptr<std::list<Fmi::DateTime>>;

  // Methods

  TimeSeriesGeneratorOptions(const Fmi::DateTime& now = Fmi::SecondClock::universal_time());

  std::size_t hash_value() const;

  // All timesteps are to be used?
  bool all() const;

  // Handle data times
  void setDataTimes(const TimeList& times, bool climatology = false);
  const TimeList& getDataTimes() const;

  static TimeSeriesGeneratorOptions parse(
    const Spine::HTTP::Request& theReq,
    const Defaults& defaults = Defaults(),
    const Fmi::DateTime& now = Fmi::SecondClock::universal_time());

  Mode mode = Mode::TimeSteps;              // algorithm selection
  Fmi::DateTime startTime;                  // start time
  Fmi::DateTime endTime;                    // end time
  bool startTimeUTC = true;                 // timestamps can be interpreted to be in
  bool endTimeUTC = true;                   // UTC time or in some specific time zone
  boost::optional<unsigned int> timeSteps;  // number of time steps
  boost::optional<unsigned int> timeStep;   // Mode:TimeSteps, timestep in Fmi::Minutes
  std::set<unsigned int> timeList;          // Mode:FixedTimes,  integers of form HHMM
  std::set<unsigned int> days;

 private:
  void parse_day(const Spine::HTTP::Request& theReq);
  void parse_hour(const Spine::HTTP::Request& theReq);
  void parse_time(const Spine::HTTP::Request& theReq);
  void parse_timestep(const Spine::HTTP::Request& theReq);
  void parse_timesteps(const Spine::HTTP::Request& theReq);
  void parse_starttime(const Spine::HTTP::Request& theReq, const Defaults& defaults);
  void parse_startstep(const Spine::HTTP::Request& theReq, const Defaults& defaults);
  void parse_endtime(const Spine::HTTP::Request& theReq, const Defaults& defaults);

  void parseImpl(const Spine::HTTP::Request& theReq,
                 const Defaults& defaults);

  TimeList dataTimes;  // Mode:DataTimes, Fixed times set from outside
  Fmi::DateTime now;
 public:
  bool startTimeData = false;  // Take start time from data
  bool endTimeData = false;    // Take end time from data
  bool isClimatology = false;
  bool startTimeAssumed = false;  // Start time is not provided and the default value has been used
};

TimeSeriesGeneratorOptions parseTimes(
  const Spine::HTTP::Request& theReq,
  const TimeSeriesGeneratorOptions::Defaults& defaults = TimeSeriesGeneratorOptions::Defaults());

std::ostream& operator<<(std::ostream& stream, const TimeSeriesGeneratorOptions& opt);

}  // namespace TimeSeries
}  // namespace SmartMet

// ======================================================================
