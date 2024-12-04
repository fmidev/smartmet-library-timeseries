#include "StationParameters.h"
#include "ParameterKeywords.h"
#include <macgyver/Exception.h>
#include <macgyver/StringConversion.h>

namespace SmartMet
{
namespace TimeSeries
{

bool is_station_parameter(const std::string& paramname)
{
    return SpecialParameters::StationParameters::instance.contains(paramname);
}

Value station_parameter(const std::string& paramname,
                        const Spine::Station& station,
                        const std::string& timezone,
                        const std::string& language_code)
try
{
    const std::string name = Fmi::ascii_tolower_copy(paramname);
    SpecialParameters::StationParameterArgs args(station, timezone, language_code);
    return SpecialParameters::StationParameters::instance(name, args);
}
catch (...)
{
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
}


namespace SpecialParameters
{

const StationParameters StationParameters::instance;

StationParameters::StationParameters()
try
    : Fmi::FunctionMap<Value, StationParameterArgs>()
{
    add (DIRECTION_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            Value value = None();
            if (args.station.stationDirection >= 0)
            {
                Fmi::ValueFormatterParam vfp;
                Fmi::ValueFormatter valueFormatter(vfp);
                value = valueFormatter.format(args.station.stationDirection, 1);
            }
            return value;
        });

    add (DISTANCE_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.distance.empty())
                return None();
            return args.station.distance;
        });

    add (STATION_ELEVATION_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            return args.station.elevation;
        });

    add ({STATIONTYPE_PARAM, MODEL_PARAM},
        [](const StationParameterArgs& args) -> Value
        {
            return args.station.type;
        });

    add (FMISID_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.fmisid > 0)
                return args.station.fmisid;
            return None();
        });

    add (GEOID_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.geoid > 0)
                return args.station.geoid;
            return None();
        });

    add (LPNN_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.lpnn > 0)
                return args.station.lpnn;
            return None();
        });

    add (PLACE_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            return args.station.tag;
        });

    add (RWSID_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.rwsid > 0)
                return args.station.rwsid;
            return None();
        });

    // FIXME: is this correct?
    add (SENSOR_NO_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            return 1;
        });

    add (STATIONARY_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            return int(args.station.isStationary);
        });

    add ({LATITUDE_PARAM, STATIONLATITUDE_PARAM},
        [](const StationParameterArgs& args) -> Value
        {
            return args.station.latitude;
        });

    add ({LONGITUDE_PARAM, STATIONLONGITUDE_PARAM},
        [](const StationParameterArgs& args) -> Value
        {
            return args.station.longitude;
        });

    add (REGION_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.region.empty())
                return None();
            return args.station.region;
        });

    add (COUNTRY_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.country.empty())
                return None();
            return args.station.country;
        });

    add (ISO2_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.iso2.empty())
                return None();
            return args.station.iso2;
        });

    add (MODIFIED_LAST_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            return Fmi::to_iso_string(args.station.modified_last);
        });

    add (NAME_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            return args.station.modified_by;
        });

    add (STATION_NAME_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            return args.station.station_formal_name(args.language_code);
        });

    add (TZ_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.timezone == "localtime")
                return args.station.timezone;
            return args.timezone;
        });

    add (WMO_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (args.station.wmo > 0)
                return args.station.wmo;
            return None();
        });

    add (WSI_PARAM,
        [](const StationParameterArgs& args) -> Value
        {
            if (!args.station.wsi.empty())
                return args.station.wsi;
            return None();
        });
}
catch (...)
{
    Fmi::Exception error(BCP, "Failed to initializer StationParameters singleton");
}

StationParameters::~StationParameters() = default;

} // namespace SpecialParameters
}  // namespace TimeSeries
}  // namespace SmartMet
