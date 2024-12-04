#include "LocationParameters.h"
#include "ParameterKeywords.h"
#include "TimeSeriesOutput.h"
#include <macgyver/StringConversion.h>
#include <gis/CoordinateTransformation.h>
#include <gis/SpatialReference.h>
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

  Value result = LocationParameters::instance(
            Fmi::ascii_tolower_copy(paramName),
            args,
            precision);

  return std::visit(visitor, result);
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

} // anonymous namespace


const LocationParameters LocationParameters::instance;

struct LocationParameterArgs::State
{
    // Add member variables here
};

LocationParameterArgs::~LocationParameterArgs()
{
    delete state;
}


LocationParameters::LocationParameters()
{
    add(COUNTRY_PARAM,
        [](LocationParameterArgs& args, int) -> Value
        {
            return args.loc.country;
        });

    add(COUNTRY_PARAM,
        [](LocationParameterArgs& args, int) -> Value
        {
            return args.loc.country;
        });

    add(DEM_PARAM,
        [](LocationParameterArgs& args, int precision) -> Value
        {
            if (std::isnan(args.loc.dem))
                return Spine::None();
            return args.value_formatter.format(args.loc.dem, precision);
        }
    );

    add(ELEVATION_PARAM,
        [](LocationParameterArgs& args, int precision) -> Value
        {
            return args.value_formatter.format(args.loc.elevation, precision);
        });

    add(FEATURE_PARAM,
        [](LocationParameterArgs& args, int) -> Value
        {
            return args.loc.feature;
        });

    add(FMISID_PARAM,
        [](LocationParameterArgs& args, int) -> Value
        {
            if (args.loc.fmisid)
                return *args.loc.fmisid;
            return Spine::None();
        });

    add(GEOID_PARAM,
        [](LocationParameterArgs& args, int precision) -> Value
        {
            return args.value_formatter.format(args.loc.geoid, precision);
        });

    add(ISO2_PARAM,
        [](LocationParameterArgs& args, int) -> Value
        {
            return args.loc.iso2;
        });

    add({LAT_PARAM, LATITUDE_PARAM},
        [](LocationParameterArgs& args, int precision) -> Value
        {
            return args.value_formatter.format(args.loc.latitude, precision);
        });

    add(LATLON_PARAM,
        [](LocationParameterArgs& args, int precision) -> Value
        {
            return args.value_formatter.format(args.loc.latitude, precision) + ", " +
                   args.value_formatter.format(args.loc.longitude, precision);
        });

    add(LOCALTZ_PARAM,
        [](LocationParameterArgs& args, int) -> Value
        {
            return args.loc.timezone;
        });

    add({LON_PARAM, LONGITUDE_PARAM},
        [](LocationParameterArgs& args, int precision) -> Value
        {
            return args.value_formatter.format(args.loc.longitude, precision);
        });


    add(LONLAT_PARAM,
        [](LocationParameterArgs& args, int precision) -> Value
        {
            return args.value_formatter.format(args.loc.longitude, precision) + ", " +
                   args.value_formatter.format(args.loc.latitude, precision);
        });

    add(NAME_PARAM,
        [](LocationParameterArgs& args, int) -> Value
        {
            return args.loc.name;
        });

    add(POPULATION_PARAM,
        [](LocationParameterArgs& args, int) -> Value
        {
            if (args.loc.population >= 0)
                return args.loc.population;
            return Spine::None();
        });

    add(REGION_PARAM,
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
        });

    add(X_PARAM,
        [](LocationParameterArgs& args, int precision) -> Value
        {
            const auto [x_param, _] = x_y_param(args.loc, args.crs);
            return args.value_formatter.format(x_param, precision);
        });

    add(Y_PARAM,
        [](LocationParameterArgs& args, int precision) -> Value
        {
            const auto [_, y_param] = x_y_param(args.loc, args.crs);
            return args.value_formatter.format(y_param, precision);
        });

}

}  // namespace SpecialParameters
}  // namespace TimeSeries
}  // namespace SmartMet
