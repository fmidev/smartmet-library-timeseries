// ======================================================================
/*!
 * \brief Interface of class ValueAggregator
 */
// ======================================================================

#pragma once

#include <variant>
#include <macgyver/LocalDateTime.h>
#include <spine/LonLat.h>
#include <spine/None.h>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

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

struct Value : public Value_
{
  Value() = default;

  inline Value(const Spine::None& /* none */) : Value() {}

  inline Value(const std::string& s) : Value_(s) {}

  inline Value(const char* s) : Value_(std::string(s)) {}

  template <typename ArgType>
  inline Value(ArgType x, typename std::enable_if<std::is_integral<ArgType>::value, int>::type = 0)
      : Value_(int(x))
  {
  }

  template <typename ArgType>
  inline Value(ArgType x,
               typename std::enable_if<std::is_floating_point<ArgType>::value, int>::type = 0)
      : Value_(double(x))
  {
  }

  template <typename VisitorType>
  auto apply_visitor(VisitorType&& visitor) const
  {
    return std::visit(visitor, dynamic_cast<const Value_&>(*this));
  }

  inline Value(const Spine::LonLat& x) : Value_(x) {}

  inline Value(const Fmi::LocalDateTime& x) : Value_(x) {}

  inline Value(const Value&) = default;

  inline Value& operator=(const Value&) = default;

  bool operator==(const Value& x) const; /* { return this->Value_::operator==((Value_&)x); } */

  bool operator!=(const Value& x) const { return ! operator == (x); }
};

struct TimedValue
{
  TimedValue(const Fmi::LocalDateTime& timestamp, const Value& val)
      : time(const_cast<Fmi::LocalDateTime&>(timestamp)), value(val)
  {
  }
  TimedValue(const TimedValue& tv)
      : time(const_cast<Fmi::LocalDateTime&>(tv.time)), value(tv.value)
  {
  }
  TimedValue& operator=(const TimedValue& tv)
  {
    if (this != &tv)
    {
      time = const_cast<Fmi::LocalDateTime&>(tv.time);
      value = tv.value;
    }
    return *this;
  }

  Fmi::LocalDateTime time;
  Value value;
};

using TimedValueVector = std::vector<TimedValue>;

class TimeSeries : public TimedValueVector
{
 public:
  TimeSeries() = default;
  TimeSeries(const TimeSeries&) = default;
  void emplace_back(const TimedValue& tv);
  void push_back(const TimedValue& tv);
  TimedValueVector::iterator insert(TimedValueVector::iterator pos, const TimedValue& tv);
  void insert(TimedValueVector::iterator pos,
              TimedValueVector::iterator first,
              TimedValueVector::iterator last);
  void insert(TimedValueVector::iterator pos,
              TimedValueVector::const_iterator first,
              TimedValueVector::const_iterator last);
  TimeSeries& operator=(const TimeSeries& ts);
};

// one time series
using TimeSeriesPtr = std::shared_ptr<TimeSeries>;

// time series result variable for an area
struct LonLatTimeSeries
{
  LonLatTimeSeries(const Spine::LonLat& coord, const TimeSeries& ts) : lonlat(coord), timeseries(ts)
  {
  }
  Spine::LonLat lonlat;
  TimeSeries timeseries;
};

// several coordinate-time series pairs
using TimeSeriesGroup = std::vector<LonLatTimeSeries>;
using TimeSeriesGroupPtr = std::shared_ptr<TimeSeriesGroup>;

// time series vector
using TimeSeriesVector = std::vector<TimeSeries>;
using TimeSeriesVectorPtr = std::shared_ptr<TimeSeriesVector>;

}  // namespace TimeSeries
}  // namespace SmartMet

// ======================================================================
