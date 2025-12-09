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

  Value(const Spine::None& /* none */) : Value() {}

  Value(const std::string& s) : Value_(s) {}

  Value(const char* s) : Value_(std::string(s)) {}

  template <typename ArgType>
  Value(ArgType x, std::enable_if_t<std::is_integral_v<ArgType>, int> /* tmp */ = 0)
      : Value_(int(x))
  {
  }

  template <typename ArgType>
  Value(ArgType x, std::enable_if_t<std::is_floating_point_v<ArgType>, int> /* tmp */ = 0)
      : Value_(double(x))
  {
  }

  template <typename VisitorType>
  auto apply_visitor(VisitorType&& visitor) const
  {
    return std::visit(visitor, dynamic_cast<const Value_&>(*this));
  }

  Value(const Spine::LonLat& x) : Value_(x) {}

  Value(const Fmi::LocalDateTime& x) : Value_(x) {}

  Value(const Value&) = default;

  Value& operator=(const Value&) = default;

  bool operator==(const Value& other) const;

  bool operator!=(const Value& other) const { return !operator==(other); }

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

  LocalTimeList getTimes() const { return timeseries.getTimes(); }

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
