#pragma once

#include "OptionParsers.h"
#include "TimeSeries.h"
#include <macgyver/FunctionMap.h>
#include <macgyver/ValueFormatter.h>
#include <spine/Location.h>
#include <memory>
#include <string>

namespace SmartMet
{
namespace TimeSeries
{

bool is_plain_location_query(const OptionParsers::ParameterList& theParams);

bool is_location_parameter(const std::string& paramname);

std::string location_parameter(const Spine::LocationPtr& loc,
                               const std::string& paramName,
                               const Fmi::ValueFormatter& valueformatter,
                               const std::string& timezone,
                               int precision,
                               const std::string& crs /*="EPSG:4326"*/);

namespace SpecialParameters
{

/**
 * @brief Class for location dependent special parameter support (it holds arguments for
 *        determining special parameter values)
 */
struct LocationParameterArgs final
{
  const Spine::Location& loc;
  const Fmi::ValueFormatter& value_formatter;
  const std::string& timezone;
  const std::string& crs;

  LocationParameterArgs(const Spine::Location& loc,
                        const Fmi::ValueFormatter& value_formatter,
                        const std::string& timezone,
                        const std::string& crs)
      : loc(loc), value_formatter(value_formatter), timezone(timezone), crs(crs)
  {
  }

  ~LocationParameterArgs();

 private:
  // Currently unused, but can be used for internal caching.
  // This way changes to this class do not break the API.
  struct State;

  // std::unique_ptr does not work with incomplete types. I preffered not to
  // use std::shared_ptr<> here, because it would be an overkill (AP)
  mutable State* state = nullptr;

  // Not imlemented now
  State* get_mutable_state() const;
};

class LocationParameters final
    : public Fmi::FunctionMap<::SmartMet::TimeSeries::Value, LocationParameterArgs&, int>
{
  LocationParameters();

 public:
  static const LocationParameters instance;
};

}  // namespace SpecialParameters
}  // namespace TimeSeries
}  // namespace SmartMet
