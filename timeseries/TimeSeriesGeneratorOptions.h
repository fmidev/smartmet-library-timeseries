// ======================================================================
/*!
 * \brief Options for generating a time series
 */
// ======================================================================

#pragma once

#include <optional>
#include <memory>
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

  // Timesteps established from the outside
  using TimeList = std::shared_ptr<std::list<Fmi::DateTime>>;

  // Methods

  TimeSeriesGeneratorOptions(const Fmi::DateTime& now = Fmi::SecondClock::universal_time());

  std::size_t hash_value() const;

  // All timesteps are to be used?
  bool all() const;

  // Handle data times
  void setDataTimes(const TimeList& times, bool climatology = false);
  const TimeList& getDataTimes() const;

  Mode mode = Mode::TimeSteps;              // algorithm selection
  Fmi::DateTime startTime;                  // start time
  Fmi::DateTime endTime;                    // end time
  bool startTimeUTC = true;                 // timestamps can be interpreted to be in
  bool endTimeUTC = true;                   // UTC time or in some specific time zone
  std::optional<unsigned int> timeSteps;  // number of time steps
  std::optional<unsigned int> timeStep;   // Mode:TimeSteps, timestep in Fmi::Minutes
  std::set<unsigned int> timeList;          // Mode:FixedTimes,  integers of form HHMM
  std::set<unsigned int> days;

 private:
  TimeList dataTimes;  // Mode:DataTimes, Fixed times set from outside
 public:
  bool startTimeData = false;  // Take start time from data
  bool endTimeData = false;    // Take end time from data
  bool isClimatology = false;
};

TimeSeriesGeneratorOptions parseTimes(const Spine::HTTP::Request& theReq);

std::ostream& operator<<(std::ostream& stream, const TimeSeriesGeneratorOptions& opt);

}  // namespace TimeSeries
}  // namespace SmartMet

// ======================================================================
