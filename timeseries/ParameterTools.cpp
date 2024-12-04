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
