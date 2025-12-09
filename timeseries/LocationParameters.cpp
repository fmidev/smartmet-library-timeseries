#include "LocationParameters.h"
#include "ParameterKeywords.h"
#include "TimeSeriesOutput.h"
#include <gis/CoordinateTransformation.h>
#include <gis/SpatialReference.h>
#include <macgyver/StringConversion.h>
#include <spine/None.h>
#include <cmath>
#include <tuple>

namespace SmartMet
{
namespace TimeSeries
{

// ----------------------------------------------------------------------
/*!
 * \brief Check if the parameter depends on location only (not time)
 */
// ----------------------------------------------------------------------

bool is_location_parameter(const std::string& paramname)
{
  return SpecialParameters::LocationParameters::instance.contains(paramname);
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
 * \brief Handle a location dependent parameter
 */
// ----------------------------------------------------------------------

std::string location_parameter(const Spine::LocationPtr& loc,
                               const std::string& paramName,
                               const Fmi::ValueFormatter& valueformatter,
                               const std::string& timezone,
                               int precision,
                               const std::string& crs /*="EPSG:4326"*/)
try
{
  using namespace SpecialParameters;
  StringVisitor visitor(valueformatter, precision);
  LocationParameterArgs args(*loc, valueformatter, timezone, crs);

  Value result = LocationParameters::instance(Fmi::ascii_tolower_copy(paramName), args, precision);

  return std::visit(visitor, dynamic_cast<Value_&>(result));
}
catch (...)
{
  throw Fmi::Exception::Trace(BCP, "Operation failed!");
}

// ----------------------------------------------------------------------
namespace SpecialParameters
{

namespace
{

auto x_y_param(const Spine::Location& loc, const std::string& crs) -> std::tuple<double, double>
{
  try
  {
    double x_coord = loc.longitude;
    double y_coord = loc.latitude;
    if (!(crs.empty() || crs == "EPSG:4326"))
    {
      Fmi::CoordinateTransformation transformation("WGS84", crs);
      transformation.transform(x_coord, y_coord);
    }

    return {x_coord, y_coord};
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // anonymous namespace

const LocationParameters LocationParameters::instance;

struct LocationParameterArgs::State
{
  // Add member variables here
};

LocationParameterArgs::~LocationParameterArgs()
{
  delete state;
}

LocationParameters::~LocationParameters() = default;

LocationParameters::LocationParameters()
{
  add(
      COUNTRY_PARAM,
      [](LocationParameterArgs& args, int) -> Value { return args.loc.country; },
      "Full country name in requested language (e.g. 'Finland')");

  add(
      DEM_PARAM,
      [](LocationParameterArgs& args, int precision) -> Value
      {
        if (std::isnan(args.loc.dem))
          return Spine::None();
        return args.value_formatter.format(args.loc.dem, precision);
      },
      "Digital Elevation Model height value for the coordinate, model may vary");

  add(
      ELEVATION_PARAM,
      [](LocationParameterArgs& args, int precision) -> Value
      { return args.value_formatter.format(args.loc.elevation, precision); },
      "Elevation of site in meters from GeoNames database");

  add(
      FEATURE_PARAM,
      [](LocationParameterArgs& args, int) -> Value { return args.loc.feature; },
      "GeoNames feature code for the location");

  add(
      FMISID_PARAM,
      [](LocationParameterArgs& args, int) -> Value
      {
        if (args.loc.fmisid)
          return *args.loc.fmisid;
        return Spine::None();
      },
      "Finnish Meteorological Institute numeric ID for the station");

  add(
      GEOID_PARAM,
      [](LocationParameterArgs& args, int) -> Value { return args.loc.geoid; },
      "GeoNames ID for the location");

  add(
      ISO2_PARAM,
      [](LocationParameterArgs& args, int) -> Value { return args.loc.iso2; },
      "Country code in ISO 3166-1 alpha-2 format");

  add(
      {LAT_PARAM, LATITUDE_PARAM},
      [](LocationParameterArgs& args, int precision) -> Value
      {
        int latlon_precision = std::max(5, precision);
        return args.value_formatter.format(args.loc.latitude, latlon_precision);
      },
      "Site latitude in degrees");

  add(
      LATLON_PARAM,
      [](LocationParameterArgs& args, int precision) -> Value
      {
        int latlon_precision = std::max(5, precision);
        return args.value_formatter.format(args.loc.latitude, latlon_precision) + ", " +
               args.value_formatter.format(args.loc.longitude, latlon_precision);
      },
      "Site latitude and longitude in degrees separated by comma");

  add(
      LOCALTZ_PARAM,
      [](LocationParameterArgs& args, int) -> Value { return args.loc.timezone; },
      "Location timezone");

  add(
      {LON_PARAM, LONGITUDE_PARAM},
      [](LocationParameterArgs& args, int precision) -> Value
      {
        int latlon_precision = std::max(5, precision);
        return args.value_formatter.format(args.loc.longitude, latlon_precision);
      },
      "Site longitude in degrees");

  add(
      LONLAT_PARAM,
      [](LocationParameterArgs& args, int precision) -> Value
      {
        int latlon_precision = std::max(5, precision);
        return args.value_formatter.format(args.loc.longitude, latlon_precision) + ", " +
               args.value_formatter.format(args.loc.latitude, latlon_precision);
      },
      "Site longitude and latitude in degrees separated by comma");

  add(
      NAME_PARAM,
      [](LocationParameterArgs& args, int) -> Value { return args.loc.name; },
      "GeoNames name of the location");

  add(
      POPULATION_PARAM,
      [](LocationParameterArgs& args, int) -> Value
      {
        if (args.loc.population >= 0)
          return args.loc.population;
        return Spine::None();
      },
      "Population of the location (if available)");

  add(
      REGION_PARAM,
      [](LocationParameterArgs& args, int) -> Value
      {
        if (args.loc.area.empty())
        {
          if (args.loc.name.empty())
          {
            // No area (administrative region) nor name known.
            return args.value_formatter.missing();
          }
          // Place name known, administrative region unknown.
          return args.loc.name;
        }
        // Administrative region known.
        return args.loc.area;
      },
      "Region/area name of the location if available, name otherwise");

  add(
      X_PARAM,
      [](LocationParameterArgs& args, int precision) -> Value
      {
        const auto [x_param, _] = x_y_param(args.loc, args.crs);
        return args.value_formatter.format(x_param, precision);
      },
      "X-coordinate in the requested projection");

  add(
      Y_PARAM,
      [](LocationParameterArgs& args, int precision) -> Value
      {
        const auto [_, y_param] = x_y_param(args.loc, args.crs);
        return args.value_formatter.format(y_param, precision);
      },
      "Y-coordinate in the requested projection");
}

}  // namespace SpecialParameters
}  // namespace TimeSeries
}  // namespace SmartMet
