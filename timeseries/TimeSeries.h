// ======================================================================
/*!
 * \brief Interface of class ValueAggregator
 */
// ======================================================================

#pragma once

#include "TimeSeriesTypes.h"
#include <macgyver/LocalDateTime.h>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace SmartMet
{
namespace TimeSeries
{

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

  bool operator!=(const Value& x) const { return !operator==(x); }

  /**
   *   @brief Get double value using supported conversions
   */
  double as_double() const;

  int as_int() const;
};

struct TimedValue
{
  TimedValue(const Fmi::LocalDateTime& timestamp, const Value& val)
      : time(const_cast<Fmi::LocalDateTime&>(timestamp)), value(val)
  {
  }
  TimedValue(const TimedValue& tv) : time(const_cast<Fmi::LocalDateTime&>(tv.time)), value(tv.value)
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

  LocalTimeList getTimes() const;
};

// one time series
using TimeSeriesPtr = std::shared_ptr<TimeSeries>;

// time series result variable for an area
struct LonLatTimeSeries
{
  LonLatTimeSeries(const Spine::LonLat& coord, const TimeSeries& ts) : lonlat(coord), timeseries(ts)
  {
  }

  inline LocalTimeList getTimes() const { return timeseries.getTimes(); }

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
