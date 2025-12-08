#pragma once

#include "TimeSeries.h"
#include <macgyver/FunctionMap.h>
#include <macgyver/LocalDateTime.h>
#include <macgyver/TimeFormatter.h>
#include <macgyver/TimeZones.h>
#include <spine/Location.h>

namespace SmartMet
{
namespace TimeSeries
{

bool is_time_parameter(std::string paramname);

Value time_parameter(const std::string& paramname,
                     const Fmi::LocalDateTime& ldt,
                     const Fmi::DateTime& now,
                     const Spine::Location& loc,
                     const std::string& timezone,
                     const Fmi::TimeZones& timezones,
                     const std::locale& outlocale,
                     const Fmi::TimeFormatter& timeformatter,
                     const std::string& timestring);

namespace SpecialParameters
{

struct TimeParameterArgs final
{
  const Fmi::LocalDateTime& ldt;
  const Fmi::DateTime& now;
  const Spine::Location& loc;
  const std::string& timezone;
  const Fmi::TimeZones& timezones;
  const std::locale& outlocale;
  const Fmi::TimeFormatter& timeformatter;
  const std::string& timestring;

  TimeParameterArgs(const Fmi::LocalDateTime& ldt,
                    const Fmi::DateTime& now,
                    const Spine::Location& loc,
                    const std::string& timezone,
                    const Fmi::TimeZones& timezones,
                    const std::locale& outlocale,
                    const Fmi::TimeFormatter& timeformatter,
                    const std::string& timestring)
      : ldt(ldt),
        now(now),
        loc(loc),
        timezone(timezone),
        timezones(timezones),
        outlocale(outlocale),
        timeformatter(timeformatter),
        timestring(timestring)
  {
  }

  ~TimeParameterArgs();

 private:
  // Currently unused, but can be used for internal caching of temporary values to
  // improve performance. This way changes to this structure will not break the API.
  struct State;

  // std::unique_ptr does not work with incomplete types. I preffered not to
  // use std::shared_ptr<> here, because it would be an overkill (AP)
  mutable State* state = nullptr;

  // Not implemented now
  State* get_mutable_state() const;
};

/**
 *  @brief Class for time dependent special parameter support
 */
class TimeParameters final :
    // HUOM: fully quallified names of TimeSeries::Value is being used here to avoid
    //       possible ambiguity with SmartMet::Spine::Value when both namespaces
    //       are included in the same scope.
    public Fmi::FunctionMap<::SmartMet::TimeSeries::Value, TimeParameterArgs&>
{
  TimeParameters();

 public:
  static const TimeParameters instance;
};

}  // namespace SpecialParameters
}  // namespace TimeSeries
}  // namespace SmartMet
