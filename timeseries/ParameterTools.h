#pragma once

#include "OptionParsers.h"
#include "TimeSeriesInclude.h"
#include "LocationParameters.h"
#include "TimeParameters.h"
#include <macgyver/TimeFormatter.h>
#include <macgyver/TimeZones.h>
#include <newbase/NFmiParameterName.h>
#include <spine/Location.h>
#include <spine/Parameter.h>
#include <map>
#include <set>
#include <string>
namespace Fmi
{
class ValueFormatter;
}

namespace SmartMet
{
namespace TimeSeries
{
bool special(const Spine::Parameter& theParam);

std::vector<std::string> special_parameters();

bool parameter_is_arithmetic(const Spine::Parameter& theParameter);

bool is_data_independent(const Spine::Parameter& theParam);

bool is_data_independent_query(const OptionParsers::ParameterList& theParams);

std::string location_parameter(const Spine::LocationPtr& loc,
                               const std::string& paramName,
                               const Fmi::ValueFormatter& valueformatter,
                               const std::string& timezone,
                               int precision,
                               const std::string& crs = "EPSG:4326");

Spine::Parameter makeParameter(const std::string& name);

bool is_special_parameter(const std::string& name);

Spine::Parameter get_query_param(const Spine::Parameter& parameter);
void transform_wgs84_coordinates(const std::string& name,
                                 const std::string& target_crs,
                                 const Spine::Location& loc,
                                 TS::TimeSeries& tseries);
void transform_wgs84_coordinates(const std::string& name,
                                 const std::string& crs,
                                 TS::TimeSeriesGroup& tsg);
std::string get_parameter_id(const Spine::Parameter& parameter);

/**
 * @brief Special parameters related definitions
 *
 * These are used in the implementation of the special parameters.
 * User should generally use the methods declared above instead of these
 * directly.
 */
namespace SpecialParameter
{
    //extern const std::set<std::string> location_parameters;
    extern const std::map<std::string, Spine::Parameter::Type> special_parameter_map;
    extern const std::map<std::string, FmiParameterName> time_parameter_map;
    extern const std::map<std::string, FmiParameterName> location_parameter_map;
}  // namespace SpecialParameter

}  // namespace TimeSeries
}  // namespace SmartMet
