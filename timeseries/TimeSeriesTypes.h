#pragma once

#include <list>
#include <variant>
#include <macgyver/LocalDateTime.h>
#include <spine/LonLat.h>
#include <spine/None.h>

namespace SmartMet
{
namespace TimeSeries
{

using None = ::SmartMet::Spine::None;
using LonLat = ::SmartMet::Spine::LonLat;
using LonLatFormat = ::SmartMet::Spine::LonLatFormat;

// data variable for qengine, obsengine
using Value_ = std::variant<Spine::None,
                              std::string,
                              double,
                              int,
                              Spine::LonLat,
                              Fmi::LocalDateTime>;

using LocalTimeList = std::list<Fmi::LocalDateTime>;

}  // namespace TimeSeries
}  // namespace SmartMet
