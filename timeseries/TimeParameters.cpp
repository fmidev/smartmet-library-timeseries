#include "TimeParameters.h"
#include "ParameterKeywords.h"
#include "TimeSeries.h"
#include <macgyver/Astronomy.h>
#include <macgyver/NumericCast.h>


namespace SmartMet
{
namespace TimeSeries
{

using namespace SpecialParameters;

bool is_time_parameter(std::string paramname)
{
    const std::string p = Fmi::ascii_tolower_copy(paramname);
    if (TimeParameters::instance.contains(p))
        return true;
    return false;
}

Value time_parameter(const std::string& paramname,
                         const Fmi::LocalDateTime& ldt,
                         const Fmi::DateTime& now,
                         const Spine::Location& loc,
                         const std::string& timezone,
                         const Fmi::TimeZones& timezones,
                         const std::locale& outlocale,
                         const Fmi::TimeFormatter& timeformatter,
                         const std::string& timestring)
try
{
    // Special case for date() parameters (we should not lowercase the format string)
    std::string name = Fmi::ascii_tolower_copy(paramname);
    if (strncasecmp(paramname.c_str(), "date(", 5) == 0)
        name = "date(" + paramname.substr(5);
    else
        name = Fmi::ascii_tolower_copy(paramname);

    TimeParameterArgs args(ldt, now, loc, timezone, timezones, outlocale, timeformatter, timestring);
    return TimeParameters::instance(name, args);
}
catch (...)
{
    auto error = Fmi::Exception::Trace(BCP, "Operation failed!");
    error.addParameter("Parameter", paramname);
    throw error;
}

namespace SpecialParameters
{

const TimeParameters TimeParameters::instance;

struct TimeParameterArgs::State
{
    // Add member variables here
};

TimeParameterArgs::~TimeParameterArgs()
{
    delete state;
}

TimeParameters::TimeParameters()
{
    add (DARK_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_position_t sp =
                Fmi::Astronomy::solar_position(args.ldt, args.loc.longitude, args.loc.latitude);
            const std::string ret = Fmi::to_string(sp.dark());
            return Value(ret);
        });

    add (DAYLENGTH_PARAM,
        [](TimeParameterArgs& args)
        {
          Fmi::Astronomy::solar_time_t st =
              Fmi::Astronomy::solar_time(args.ldt, args.loc.longitude, args.loc.latitude);
          auto seconds = st.daylength().total_seconds();
          int minutes = Fmi::numeric_cast<int>(round(static_cast<double>(seconds) / 60.0));
          return Value(minutes);
        });

    add (EPOCHTIME_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::DateTime time_t_epoch(Fmi::Date(1970, 1, 1));
            Fmi::TimeDuration diff = args.ldt.utc_time() - time_t_epoch;
            return Value(diff.total_seconds());
        });

    add (HOUR_PARAM,
        [](TimeParameterArgs& args)
        {
            const int hour = args.ldt.local_time().time_of_day().hours();
            return Value(hour);
        });

    add (ISOTIME_PARAM,
        [](TimeParameterArgs& args)
        {
            return Value(Fmi::to_iso_string(args.ldt.local_time()));
        });

    add (LOCALTIME_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::TimeZonePtr localtz = args.timezones.time_zone_from_string(args.loc.timezone);
            const Fmi::DateTime utc = args.ldt.utc_time();
            const Fmi::LocalDateTime localt(utc, localtz);
            return Value(args.timeformatter.format(localt));
        });

    add (MON_PARAM,
        [](TimeParameterArgs& args)
        {
            const std::string ret = Fmi::format_time(args.outlocale, "%b", args.ldt);
            return Value(ret);
        });

    add (MONTH_PARAM,
        [](TimeParameterArgs& args)
        {
            const std::string ret = Fmi::format_time(args.outlocale, "%B", args.ldt);
            return Value(ret);
        });

    add (MOONDOWN24H_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            const auto ret = Fmi::to_string(!lt.moonrise_today() && !lt.moonset_today() &&
                                            !lt.above_horizont_24h());
            return Value(ret);
        });

    add (MOONPHASE_PARAM,
        [](TimeParameterArgs& args)
        {
            const double result = Fmi::Astronomy::moonphase(args.ldt.utc_time());
            return Value(result);
       });

    add (MOONRISE_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            if (lt.moonrise_today())
                return Value(args.timeformatter.format(lt.moonrise.local_time()));
            return Value();
        });

    add (MOONRISE2_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            if (lt.moonrise2_today())
                return Value(args.timeformatter.format(lt.moonrise2.local_time()));
            return Value();
        });

    add (MOONRISE2TODAY_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(Fmi::to_string(lt.moonrise2_today()));
        });

    add (MOONRISETODAY_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(Fmi::to_string(lt.moonrise_today()));
        });

    add (MOONSET_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            if (lt.moonset_today())
                return Value(args.timeformatter.format(lt.moonset.local_time()));
            return Value();
        });

    add (MOONSET2_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            if (lt.moonset2_today())
                return Value(args.timeformatter.format(lt.moonset2.local_time()));
            return Value();
        });

    add (MOONSET2TODAY_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(Fmi::to_string(lt.moonset2_today()));
        });

    add (MOONSETTODAY_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(Fmi::to_string(lt.moonset_today()));
        });

    add (MOONUP24H_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::lunar_time_t lt =
                Fmi::Astronomy::lunar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(Fmi::to_string(lt.above_horizont_24h()));
        });

    add (NOON_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_time_t st =
                Fmi::Astronomy::solar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(args.timeformatter.format(st.noon.local_time()));
        });

    add (ORIGINTIME_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::TimeZonePtr tz = args.timezones.time_zone_from_string(args.timezone);
            Fmi::LocalDateTime ldt_now(args.now, tz);
            return Value(Fmi::to_iso_string(ldt_now.local_time()));
        });

    add (SUNAZIMUTH_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_position_t sp =
                Fmi::Astronomy::solar_position(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(sp.azimuth);
        });

    add (SUNDECLINATION_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_position_t sp =
                Fmi::Astronomy::solar_position(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(sp.declination);
        });

    add (SUNELEVATION_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_position_t sp =
                Fmi::Astronomy::solar_position(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(sp.elevation);
        });

    add (SUNRISE_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_time_t st =
                Fmi::Astronomy::solar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(args.timeformatter.format(st.sunrise.local_time()));
        });

    add (SUNRISETODAY_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_time_t st =
                Fmi::Astronomy::solar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(Fmi::to_string(st.sunrise_today()));
        });

    add (SUNSET_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_time_t st =
                Fmi::Astronomy::solar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(args.timeformatter.format(st.sunset.local_time()));
        });

    add (SUNSETTODAY_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_time_t st =
                Fmi::Astronomy::solar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(Fmi::to_string(st.sunset_today()));
        });

    add (SUNUP24H_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_time_t st =
                Fmi::Astronomy::solar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(Fmi::to_string(st.polar_day()));
        });

    add (SUNDOWN24H_PARAM,
        [](TimeParameterArgs& args)
        {
            Fmi::Astronomy::solar_time_t st =
                Fmi::Astronomy::solar_time(args.ldt, args.loc.longitude, args.loc.latitude);
            return Value(Fmi::to_string(st.polar_night()));
        });

    add (TIME_PARAM,
        [](TimeParameterArgs& args)
        {
            return Value(args.timeformatter.format(args.ldt));
        });

    add (TIMESTRING_PARAM,
        [](TimeParameterArgs& args)
        {
            return Value(Fmi::format_time(args.outlocale, args.timestring, args.ldt));
        });

    add (TZ_PARAM,
        [](TimeParameterArgs& args)
        {
            return Value(args.timezone);
        });

    add (UTCTIME_PARAM,
        [](TimeParameterArgs& args)
        {
            return Value(args.timeformatter.format(args.ldt.utc_time()));
        });

    add (WDAY_PARAM,
        [](TimeParameterArgs& args)
        {
            return Value(Fmi::format_time(args.outlocale, "%a", args.ldt));
        });

    add (WEEKDAY_PARAM,
        [](TimeParameterArgs& args)
        {
            return Value(Fmi::format_time(args.outlocale, "%A", args.ldt));
        });

    add (XMLTIME_PARAM,
        [](TimeParameterArgs& args)
        {
            return Value(Fmi::to_iso_extended_string(args.ldt.local_time()));
        });

    add("date(format)", boost::regex("date\\(([^)]+)\\)"),
        [](const std::vector<std::string>& r_arg, TimeParameterArgs& args)
        {
            const std::string fmt = r_arg.at(0);
            return Value(Fmi::format_time(args.outlocale, fmt, args.ldt));
        });
}

}  // namespace SpecialParameters
}  // namespace TimeSeries
}  // namespace SmartMet
