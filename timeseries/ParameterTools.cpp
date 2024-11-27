#include "ParameterTools.h"
#include "ParameterKeywords.h"
#include <boost/algorithm/string.hpp>
#include <gis/CoordinateTransformation.h>
#include <gis/SpatialReference.h>
#include <macgyver/Astronomy.h>
#include <macgyver/CharsetTools.h>
#include <macgyver/NumericCast.h>
#include <macgyver/StringConversion.h>
#include <macgyver/TimeFormatter.h>
#include <macgyver/ValueFormatter.h>
#include <spine/None.h>
#include <algorithm>
#include <locale>
#include <set>

namespace SmartMet
{
namespace TimeSeries
{
namespace SpecialParameters
{
using Spine::Parameter;

const std::set<std::string> location_parameters = {
                                                   COUNTRY_PARAM,
                                                   DEM_PARAM,
                                                   ELEVATION_PARAM,
                                                   FEATURE_PARAM,
                                                   FMISID_PARAM,
                                                   GEOID_PARAM,
                                                   ISO2_PARAM,
                                                   LAT_PARAM,
                                                   LATITUDE_PARAM,
                                                   LATLON_PARAM,
                                                   // LEVEL_PARAM,
                                                   LOCALTZ_PARAM,
                                                   LON_PARAM,
                                                   LONGITUDE_PARAM,
                                                   LONLAT_PARAM,
                                                   NAME_PARAM,
                                                   // PLACE_PARAM,
                                                   POPULATION_PARAM,
                                                   REGION_PARAM,
                                                   STATION_ELEVATION_PARAM,
                                                   STATIONLAT_PARAM,
                                                   STATIONLATITUDE_PARAM,
                                                   STATIONLON_PARAM,
                                                   STATIONLONGITUDE_PARAM,
                                                   STATIONNAME_PARAM,
                                                   X_PARAM,
                                                   Y_PARAM
                                                   };


const std::map<std::string, Parameter::Type> special_parameter_map = {
    {"cloudceiling", Parameter::Type::DataDerived},
    {"cloudceilingft", Parameter::Type::DataDerived},
    {"cloudceilinghft", Parameter::Type::DataDerived},
    {"cloudiness8th", Parameter::Type::DataDerived},
    {COUNTRY_PARAM, Parameter::Type::DataIndependent},
    {DARK_PARAM, Parameter::Type::DataIndependent},
    {DEM_PARAM, Parameter::Type::DataIndependent},
    {DIRECTION_PARAM, Parameter::Type::DataIndependent},
    {DISTANCE_PARAM, Parameter::Type::DataIndependent},
    {ELEVATION_PARAM, Parameter::Type::DataIndependent},
    {EPOCHTIME_PARAM, Parameter::Type::DataIndependent},
    {"feelslike", Parameter::Type::DataDerived},
    {FMISID_PARAM, Parameter::Type::DataIndependent},
    {GEOID_PARAM, Parameter::Type::DataIndependent},
    {HOUR_PARAM, Parameter::Type::DataIndependent},
    {ISO2_PARAM, Parameter::Type::DataIndependent},
    {ISOTIME_PARAM, Parameter::Type::DataIndependent},
    {LAT_PARAM, Parameter::Type::DataIndependent},
    {LATITUDE_PARAM, Parameter::Type::DataIndependent},
    {LATLON_PARAM, Parameter::Type::DataIndependent},
    //{ LEVEL_PARAM, Parameter::Type::DataIndependent },
    {LOCALTIME_PARAM, Parameter::Type::DataIndependent},
    {LOCALTZ_PARAM, Parameter::Type::DataIndependent},
    {LON_PARAM, Parameter::Type::DataIndependent},
    {LONGITUDE_PARAM, Parameter::Type::DataIndependent},
    {LONLAT_PARAM, Parameter::Type::DataIndependent},
    {LPNN_PARAM, Parameter::Type::DataIndependent},
    {MODEL_PARAM, Parameter::Type::DataIndependent},
    {"modtime", Parameter::Type::DataIndependent},
    {MON_PARAM, Parameter::Type::DataIndependent},
    {MONTH_PARAM, Parameter::Type::DataIndependent},
    {MOONPHASE_PARAM, Parameter::Type::DataIndependent},
    {NAME_PARAM, Parameter::Type::DataIndependent},
    {NOON_PARAM, Parameter::Type::DataIndependent},
    {ORIGINTIME_PARAM, Parameter::Type::DataIndependent},
    {PLACE_PARAM, Parameter::Type::DataIndependent},
    {REGION_PARAM, Parameter::Type::DataIndependent},
    //{ RWSID_PARAM, Parameter::Type::DataIndependent },
    //{ SENSOR_NO_PARAM, Parameter::Type::DataIndependent },
    {"smartsymbol", Parameter::Type::DataDerived},
    //{ "stationary", Parameter::Type::DataIndependent },
    {STATION_ELEVATION_PARAM, Parameter::Type::DataIndependent},
    {STATION_NAME_PARAM, Parameter::Type::DataIndependent},
    {STATIONLAT_PARAM, Parameter::Type::DataIndependent},
    {STATIONLATITUDE_PARAM, Parameter::Type::DataIndependent},
    {STATIONLON_PARAM, Parameter::Type::DataIndependent},
    {STATIONLONGITUDE_PARAM, Parameter::Type::DataIndependent},
    {STATIONNAME_PARAM, Parameter::Type::DataIndependent},
    {STATIONTYPE_PARAM, Parameter::Type::DataIndependent},
    {SUNAZIMUTH_PARAM, Parameter::Type::DataIndependent},
    {SUNDECLINATION_PARAM, Parameter::Type::DataIndependent},
    {SUNELEVATION_PARAM, Parameter::Type::DataIndependent},
    {SUNRISE_PARAM, Parameter::Type::DataIndependent},
    {SUNRISETODAY_PARAM, Parameter::Type::DataIndependent},
    {SUNSET_PARAM, Parameter::Type::DataIndependent},
    {SUNSETTODAY_PARAM, Parameter::Type::DataIndependent},
    {TIME_PARAM, Parameter::Type::DataIndependent},
    {TIMESTRING_PARAM, Parameter::Type::DataIndependent},
    {TZ_PARAM, Parameter::Type::DataIndependent},
    {UTCTIME_PARAM, Parameter::Type::DataIndependent},
    {"weather", Parameter::Type::DataDerived},
    {"windchill", Parameter::Type::DataDerived},
    {"windcompass16", Parameter::Type::DataDerived},
    {"windcompass32", Parameter::Type::DataDerived},
    {"windcompass8", Parameter::Type::DataDerived},
    {WMO_PARAM, Parameter::Type::DataIndependent},
    {WSI_PARAM, Parameter::Type::DataIndependent},
    {X_PARAM, Parameter::Type::DataIndependent},
    {XMLTIME_PARAM, Parameter::Type::DataIndependent},
    {Y_PARAM, Parameter::Type::DataIndependent}
    };

const std::map<std::string, FmiParameterName> time_parameter_map = {
    {DARK_PARAM, kFmiDark},
    {DAYLENGTH_PARAM, kFmiDayLength},
    {EPOCHTIME_PARAM, kFmiEpochTime},
    {HOUR_PARAM, kFmiHour},
    {ISOTIME_PARAM, kFmiISOTime},
    {LOCALTIME_PARAM, kFmiLocalTime},
    {MON_PARAM, kFmiMon},
    {MONTH_PARAM, kFmiMonth},
    {MOONDOWN24H_PARAM, kFmiMoonDown24h},
    {MOONPHASE_PARAM, kFmiMoonPhase},
    {MOONRISE_PARAM, kFmiMoonrise},
    {MOONRISE2_PARAM, kFmiMoonrise2},
    {MOONRISE2TODAY_PARAM, kFmiMoonrise2Today},
    {MOONRISETODAY_PARAM, kFmiMoonriseToday},
    {MOONSET_PARAM, kFmiMoonset},
    {MOONSET2_PARAM, kFmiMoonset2},
    {MOONSET2TODAY_PARAM, kFmiMoonset2Today},
    {MOONSETTODAY_PARAM, kFmiMoonsetToday},
    {MOONUP24H_PARAM, kFmiMoonUp24h},
    {NOON_PARAM, kFmiNoon},
    {ORIGINTIME_PARAM, kFmiOriginTime},
    {SUNAZIMUTH_PARAM, kFmiSunAzimuth},
    {SUNDECLINATION_PARAM, kFmiSunDeclination},
    {SUNELEVATION_PARAM, kFmiSunElevation},
    {SUNRISE_PARAM, kFmiSunrise},
    {SUNRISETODAY_PARAM, kFmiSunriseToday},
    {SUNSET_PARAM, kFmiSunset},
    {SUNSETTODAY_PARAM, kFmiSunsetToday},
    {TIME_PARAM, kFmiTime},
    {TIMESTRING_PARAM, kFmiTimeString},
    {TZ_PARAM, kFmiTZ},
    {UTCTIME_PARAM, kFmiUTCTime},
    {WDAY_PARAM, kFmiWDay},
    {WEEKDAY_PARAM, kFmiWeekday},
    {XMLTIME_PARAM, kFmiXMLTime}
    };

const std::map<std::string, FmiParameterName> location_parameter_map = {
    {COUNTRY_PARAM, kFmiCountry},
    {DEM_PARAM, kFmiDEM},
    {ELEVATION_PARAM, kFmiElevation},
    {FEATURE_PARAM, kFmiFeature},
    {FMISID_PARAM, kFmiFMISID},
    {GEOID_PARAM, kFmiGEOID},
    {ISO2_PARAM, kFmiISO2},
    {LAT_PARAM, kFmiLatitude},
    {LATITUDE_PARAM, kFmiLatitude},
    {LATLON_PARAM, kFmiLatLon},
    {LOCALTZ_PARAM, kFmiLocalTZ},
    {LON_PARAM, kFmiLongitude},
    {LONGITUDE_PARAM, kFmiLongitude},
    {LONLAT_PARAM, kFmiLonLat},
    {NAME_PARAM, kFmiName},
    {POPULATION_PARAM, kFmiPopulation},
    {REGION_PARAM, kFmiRegion},
    {RWSID_PARAM, kFmiRWSID},
    {STATION_ELEVATION_PARAM, kFmiElevation},
    {STATIONLAT_PARAM, kFmiStationLatitude},
    {STATIONLATITUDE_PARAM, kFmiStationLatitude},
    {STATIONLON_PARAM, kFmiStationLongitude},
    {STATIONLONGITUDE_PARAM, kFmiStationLongitude},
    {STATIONNAME_PARAM, kFmiStationName},
    {WMO_PARAM, kFmiWmoStationNumber},
    {WSI_PARAM, kFmiWSI}
    };
} // namespace SpecialParameters

using namespace SpecialParameters;

namespace
{
using Spine::Parameter;

// ----------------------------------------------------------------------
/*!
 * \brief Time formatter
 */
// ----------------------------------------------------------------------

std::string format_date(const Fmi::LocalDateTime& ldt,
                        const std::locale& llocale,
                        const std::string& fmt)
{
  try
  {
    const std::string str = Fmi::date_time::format_time(llocale, fmt, ldt);
    return Fmi::latin1_to_utf8(str);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::string x_y_param(const std::string& paramName,
                      const Spine::LocationPtr& loc,
                      const std::string& crs,
                      const Fmi::ValueFormatter& valueformatter,
                      int precision)
{
  try
  {
    double x_coord = loc->longitude;
    double y_coord = loc->latitude;
    if (!(crs.empty() || crs == "EPSG:4326"))
    {
      Fmi::CoordinateTransformation transformation("WGS84", crs);
      transformation.transform(x_coord, y_coord);
    }

    if (paramName == X_PARAM)
      return valueformatter.format(x_coord, precision);

    return valueformatter.format(y_coord, precision);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // anonymous namespace

// ----------------------------------------------------------------------
/*!
 * \brief Returns vector containing the names of all special parameters
 */
// ----------------------------------------------------------------------
std::vector<std::string> special_parameters()
{
  try
  {
    std::vector<std::string> params;
    std::transform(special_parameter_map.begin(),
                   special_parameter_map.end(),
                   std::back_inserter(params),
                   [](const auto& param) { return param.first; });
    return params;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

bool special(const Parameter& theParam)
{
  try
  {
    switch (theParam.type())
    {
      case Spine::Parameter::Type::Data:
        return false;
      case Spine::Parameter::Type::DataDerived:
      case Spine::Parameter::Type::DataIndependent:
        return true;
    }
    // ** NOT REACHED **
    return true;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Return true if the parameter is data independent
 */ 
// ----------------------------------------------------------------------
bool is_data_independent(const Spine::Parameter& theParam)
{
  try
  {
    switch (theParam.type())
    {
      case Spine::Parameter::Type::Data:
      case Spine::Parameter::Type::DataDerived:
        return false;
      case Spine::Parameter::Type::DataIndependent:
        return true;
    }
    return false;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}


// ----------------------------------------------------------------------
/*!
 * \brief Return true if the query contains only data independent parameters
 */
// ----------------------------------------------------------------------
bool is_data_independent_query(const OptionParsers::ParameterList& theParams)
{
  try
  {
    return std::all_of(
      theParams.begin(),
      theParams.end(),
      [](const auto& param) { return is_data_independent(param); });
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Return true if the parameter if of aggregatable type
 */
// ----------------------------------------------------------------------

bool parameter_is_arithmetic(const Spine::Parameter& theParameter)
{
  try
  {
    switch (theParameter.type())
    {
      case Spine::Parameter::Type::Data:
      case Spine::Parameter::Type::DataDerived:
        return true;
      case Spine::Parameter::Type::DataIndependent:
        return false;
    }
    // NOT REACHED //
    return false;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Return true if the given parameters depend on location only
 */
// ----------------------------------------------------------------------

bool is_plain_location_query(const OptionParsers::ParameterList& theParams)
{
  try
  {
    for (const auto& param : theParams)
    {
      const auto& name = param.name();
      if (not is_location_parameter(name))
      {
        return false;
      }
    }
    return true;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Check if the parameter depends on location only (not time)
 */
// ----------------------------------------------------------------------

bool is_location_parameter(const std::string& paramname)
{
  return location_parameters.count(paramname) > 0;
}

// ----------------------------------------------------------------------
/*!
 * \brief Handle a location dependent parameter
 */
// ----------------------------------------------------------------------

std::string location_parameter(const Spine::LocationPtr& loc,
                               const std::string& paramName,
                               const Fmi::ValueFormatter& valueformatter,
                               const std::string& timezone,
                               int precision,
                               const std::string& crs /*="EPSG:4326"*/)
{
  try
  {
    const int latlon_precision = std::max(5, precision);
    if (!loc)
      return valueformatter.missing();

    TS::Value ret = Spine::None();

    auto p = paramName;
    Fmi::ascii_tolower(p);

    if (p == X_PARAM || p == Y_PARAM)
      return x_y_param(p, loc, crs, valueformatter, precision);

    if (location_parameter_map.find(p) == location_parameter_map.end())
      throw Fmi::Exception(BCP, "Unknown location parameter: '" + paramName + "'");

    switch (location_parameter_map.at(p))
    {
      case kFmiName:
      {
        return loc->name;
      }
      case kFmiDEM:
      {
        return valueformatter.format(loc->dem, precision);
      }
      case kFmiStationName:
      {
        return loc->name;
      }
      case kFmiISO2:
      {
        return loc->iso2;
      }
      case kFmiGEOID:
      {
        if (loc->geoid == 0)
          return valueformatter.missing();

        return Fmi::to_string(loc->geoid);
      }
      case kFmiRegion:
      {
        if (loc->area.empty())
        {
          if (loc->name.empty())
          {
            // No area (administrative region) nor name known.
            return valueformatter.missing();
          }
          // Place name known, administrative region unknown.
          return loc->name;
        }
        // Administrative region known.
        return loc->area;
      }
      case kFmiCountry:
      {
        return loc->country;
      }
      case kFmiFeature:
      {
        return loc->feature;
      }
      case kFmiLocalTZ:
      {
        return loc->timezone;
      }
      case kFmiLatitude:
      {
        return valueformatter.format(loc->latitude, latlon_precision);
      }
      case kFmiLongitude:
      {
        return valueformatter.format(loc->longitude, latlon_precision);
      }
      case kFmiLatLon:
      {
        return (valueformatter.format(loc->latitude, latlon_precision) + ", " +
                valueformatter.format(loc->longitude, latlon_precision));
      }
      case kFmiLonLat:
      {
        return (valueformatter.format(loc->longitude, latlon_precision) + ", " +
                valueformatter.format(loc->latitude, latlon_precision));
      }
      case kFmiPopulation:
      {
        return Fmi::to_string(loc->population);
      }
      case kFmiElevation:
      {
        return valueformatter.format(loc->elevation, precision);
      }
      case kFmiFMISID:
      {
        if (loc->fmisid)
          return Fmi::to_string(*loc->fmisid);
        return valueformatter.missing();
      }
      case kFmiStationLongitude:
      {
        if (loc->fmisid)
          return valueformatter.format(loc->longitude, latlon_precision);
        return valueformatter.missing();
      }
      case kFmiStationLatitude:
      {
        if (loc->fmisid)
          return valueformatter.format(loc->latitude, latlon_precision);
        return valueformatter.missing();
      }
      default:
        throw Fmi::Exception(BCP, "Unknown location parameter: '" + paramName + "'");
    }
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

bool is_time_parameter(std::string paramname)
{
  try
  {
    Fmi::ascii_tolower(paramname);
    return (time_parameter_map.find(paramname) != time_parameter_map.end() ||
            (paramname.substr(0, 5) == "date(" && paramname[paramname.size() - 1] == ')'));
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief Handle a parameter depending on time only
 */
// ----------------------------------------------------------------------

Value time_parameter(const std::string& paramname,
                     const Fmi::LocalDateTime& ldt,
                     const Fmi::DateTime& now,
                     const Spine::Location& loc,
                     const std::string& timezone,
                     const Fmi::TimeZones& timezones,
                     const std::locale& outlocale,
                     const Fmi::TimeFormatter& timeformatter,
                     const std::string& timestring)
{
  using Fmi::LocalDateTime;

  try
  {
    TS::Value ret = Spine::None();

    auto p = paramname;
    Fmi::ascii_tolower(p);

    if (time_parameter_map.find(p) != time_parameter_map.end())
    {
      switch (time_parameter_map.at(p))
      {
        case kFmiTime:
        {
          Fmi::TimeZonePtr tz = timezones.time_zone_from_string(timezone);
          ret = timeformatter.format(Fmi::LocalDateTime(ldt.utc_time(), tz));
          break;
        }
        case kFmiOriginTime:
        {
          Fmi::TimeZonePtr tz = timezones.time_zone_from_string(timezone);
          Fmi::LocalDateTime ldt_now(now, tz);
          ret = timeformatter.format(ldt_now);
          break;
        }
        case kFmiISOTime:
        {
          ret = Fmi::to_iso_string(ldt.local_time());
          break;
        }
        case kFmiXMLTime:
        {
          ret = Fmi::to_iso_extended_string(ldt.local_time());
          break;
        }
        case kFmiLocalTime:
        {
          Fmi::TimeZonePtr localtz = timezones.time_zone_from_string(loc.timezone);
          Fmi::DateTime utc = ldt.utc_time();
          Fmi::LocalDateTime localt(utc, localtz);
          ret = timeformatter.format(localt);
          break;
        }
        case kFmiUTCTime:
        {
          ret = timeformatter.format(ldt.utc_time());
          break;
        }
        case kFmiEpochTime:
        {
          Fmi::DateTime time_t_epoch(Fmi::Date(1970, 1, 1));
          Fmi::TimeDuration diff = ldt.utc_time() - time_t_epoch;
          ret = Fmi::to_string(diff.total_seconds());
          break;
        }
        case kFmiTZ:
        {
          ret = timezone;
          break;
        }
        case kFmiSunElevation:
        {
          Fmi::Astronomy::solar_position_t sp =
              Fmi::Astronomy::solar_position(ldt, loc.longitude, loc.latitude);
          ret = sp.elevation;

          break;
        }
        case kFmiSunDeclination:
        {
          Fmi::Astronomy::solar_position_t sp =
              Fmi::Astronomy::solar_position(ldt, loc.longitude, loc.latitude);
          ret = sp.declination;
          break;
        }
        case kFmiSunAzimuth:
        {
          Fmi::Astronomy::solar_position_t sp =
              Fmi::Astronomy::solar_position(ldt, loc.longitude, loc.latitude);
          ret = sp.azimuth;
          break;
        }
        case kFmiDark:
        {
          Fmi::Astronomy::solar_position_t sp =
              Fmi::Astronomy::solar_position(ldt, loc.longitude, loc.latitude);
          ret = Fmi::to_string(sp.dark());
          break;
        }
        case kFmiMoonPhase:
        {
          ret = Fmi::Astronomy::moonphase(ldt.utc_time());
          break;
        }
        case kFmiMoonrise:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          if (lt.moonrise_today())
            ret = timeformatter.format(lt.moonrise.local_time());
          break;
        }
        case kFmiMoonrise2:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          if (lt.moonrise2_today())
            ret = timeformatter.format(lt.moonrise2.local_time());
          break;
        }
        case kFmiMoonset:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          if (lt.moonset_today())
            ret = timeformatter.format(lt.moonset.local_time());
          break;
        }
        case kFmiMoonset2:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          if (lt.moonset2_today())
            ret = timeformatter.format(lt.moonset2.local_time());
          break;
        }
        case kFmiMoonriseToday:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          ret = Fmi::to_string(lt.moonrise_today());
          break;
        }
        case kFmiMoonrise2Today:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          ret = Fmi::to_string(lt.moonrise2_today());
          break;
        }
        case kFmiMoonsetToday:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          ret = Fmi::to_string(lt.moonset_today());
          break;
        }
        case kFmiMoonset2Today:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          ret = Fmi::to_string(lt.moonset2_today());
          break;
        }
        case kFmiMoonUp24h:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          ret = Fmi::to_string(lt.above_horizont_24h());
          break;
        }
        case kFmiMoonDown24h:
        {
          Fmi::Astronomy::lunar_time_t lt =
              Fmi::Astronomy::lunar_time(ldt, loc.longitude, loc.latitude);
          ret = Fmi::to_string(!lt.moonrise_today() && !lt.moonset_today() &&
                               !lt.above_horizont_24h());
          break;
        }
        case kFmiSunrise:
        {
          Fmi::Astronomy::solar_time_t st =
              Fmi::Astronomy::solar_time(ldt, loc.longitude, loc.latitude);
          ret = timeformatter.format(st.sunrise.local_time());
          break;
        }
        case kFmiSunset:
        {
          Fmi::Astronomy::solar_time_t st =
              Fmi::Astronomy::solar_time(ldt, loc.longitude, loc.latitude);
          ret = timeformatter.format(st.sunset.local_time());
          break;
        }
        case kFmiNoon:
        {
          Fmi::Astronomy::solar_time_t st =
              Fmi::Astronomy::solar_time(ldt, loc.longitude, loc.latitude);
          ret = timeformatter.format(st.noon.local_time());
          break;
        }
        case kFmiSunriseToday:
        {
          Fmi::Astronomy::solar_time_t st =
              Fmi::Astronomy::solar_time(ldt, loc.longitude, loc.latitude);
          ret = Fmi::to_string(st.sunrise_today());
          break;
        }
        case kFmiSunsetToday:
        {
          Fmi::Astronomy::solar_time_t st =
              Fmi::Astronomy::solar_time(ldt, loc.longitude, loc.latitude);
          ret = Fmi::to_string(st.sunset_today());
          break;
        }
        case kFmiDayLength:
        {
          Fmi::Astronomy::solar_time_t st =
              Fmi::Astronomy::solar_time(ldt, loc.longitude, loc.latitude);
          auto seconds = st.daylength().total_seconds();
          int minutes = Fmi::numeric_cast<int>(round(static_cast<double>(seconds) / 60.0));
          ret = minutes;
          break;
        }
        case kFmiTimeString:
        {
          ret = format_date(ldt, outlocale, timestring);
          break;
        }
        case kFmiWDay:
        {
          ret = format_date(ldt, outlocale, "%a");
          break;
        }
        case kFmiWeekday:
        {
          ret = format_date(ldt, outlocale, "%A");
          break;
        }
        case kFmiMon:
        {
          ret = format_date(ldt, outlocale, "%b");
          break;
        }
        case kFmiMonth:
        {
          ret = format_date(ldt, outlocale, "%B");
          break;
        }
        case kFmiHour:
        {
          ret = Fmi::to_string(ldt.local_time().time_of_day().hours());
          break;
        }
        default:
        {
          break;
        }
      }
    }
    else
    {
      if (p.substr(0, 5) == "date(" && p[p.size() - 1] == ')')
        ret = format_date(ldt, outlocale, paramname.substr(5, paramname.size() - 6));
    }

    return ret;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

bool is_special_parameter(const std::string& name)
{
  std::string p = boost::algorithm::to_lower_copy(name, std::locale::classic());
  return special_parameter_map.count(p) > 0;
}

Spine::Parameter makeParameter(const std::string& name)
{
  try
  {
    if (name.empty())
      throw Fmi::Exception(BCP, "Empty parameters are not allowed");

    std::string p = boost::algorithm::to_lower_copy(name, std::locale::classic());
    Spine::Parameter::Type type;

    auto it = special_parameter_map.find(p);
    if (it != special_parameter_map.end())
      type = it->second;
    else if (boost::algorithm::ends_with(p, "data_source"))
      type = Parameter::Type::DataDerived;
    else
      type = Parameter::Type::Data;

    return {name, type};
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

Spine::Parameter get_query_param(const Spine::Parameter& parameter)
{
  try
  {
    std::string paramname = parameter.name();
    std::string alias = parameter.alias();
    Spine::Parameter::Type type = parameter.type();
    FmiParameterName number = parameter.number();
    if (paramname == "x")
    {
      paramname = "lon";
      alias = "lon";
      type = Spine::Parameter::Type::DataDerived;
      number = kFmiLongitude;
    }
    else if (paramname == "y")
    {
      paramname = "lat";
      alias = "lat";
      type = Spine::Parameter::Type::DataDerived;
      number = kFmiLatitude;
    }

    return {paramname, alias, type, number};
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

void transform_wgs84_coordinates(const std::string& name,
                                 const std::string& target_crs,
                                 const Spine::Location& loc,
                                 TS::TimeSeries& tseries)
{
  try
  {
    if (target_crs.empty() || target_crs == "EPSG:4326")
      return;

    double longitude = loc.longitude;
    double latitude = loc.latitude;
    Fmi::CoordinateTransformation transformation("WGS84", target_crs);
    transformation.transform(longitude, latitude);

    if (name == "x")
    {
      for (auto& item : tseries)
        item.value = longitude;
    }
    else if (name == "y")
    {
      for (auto& item : tseries)
        item.value = latitude;
    }
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

void transform_wgs84_coordinates(const std::string& name,
                                 const std::string& crs,
                                 TS::TimeSeriesGroup& tsg)
{
  try
  {
    for (auto& item : tsg)
    {
      Spine::Location loc(item.lonlat.lon, item.lonlat.lat);
      transform_wgs84_coordinates(name, crs, loc, item.timeseries);
    }
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::string get_parameter_id(const Spine::Parameter& parameter)
{
  try
  {
    std::string ret = parameter.name();
    if (parameter.getSensorNumber())
      ret += Fmi::to_string(*(parameter.getSensorNumber()));
    const auto& sensorParameter = parameter.getSensorParameter();
    if (sensorParameter == "qc")  // later maybe longitude, latitude
      ret += sensorParameter;
    return ret;
  }
  catch (...)
  {
    throw Fmi::Exception(BCP, "Operation failed!", nullptr);
  }
}

}  // namespace TimeSeries
}  // namespace SmartMet
