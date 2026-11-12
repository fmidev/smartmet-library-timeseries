#pragma once

#include <macgyver/FunctionMap.h>
#include <string>

namespace SmartMet::Spine
{
class Station;
}  // namespace SmartMet::Spine
namespace SmartMet::TimeSeries
{
struct Value;
}  // namespace SmartMet::TimeSeries

namespace SmartMet
{
namespace TimeSeries
{

namespace SpecialParameters
{

struct StationParameterArgs
{
  StationParameterArgs(const Spine::Station& station,
                       const std::string& timezone,
                       const std::string& language_code)
      : station(station), timezone(timezone), language_code(language_code)
  {
  }

  const Spine::Station& station;
  const std::string& timezone;
  const std::string& language_code;
};

class StationParameters
    : public Fmi::FunctionMap<::SmartMet::TimeSeries::Value, StationParameterArgs>
{
 public:
  StationParameters();

  ~StationParameters() override;

  static const StationParameters instance;
};

}  // namespace SpecialParameters
}  // namespace TimeSeries
}  // namespace SmartMet
