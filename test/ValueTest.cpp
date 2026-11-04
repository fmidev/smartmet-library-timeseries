#include "TimeSeries.h"
#include "TimeSeriesOutput.h"
#include <boost/test/included/unit_test.hpp>
#include <sstream>

using SmartMet::Spine::None;
using SmartMet::TimeSeries::Value;

using namespace boost::unit_test;

test_suite* init_unit_test_suite(int argc, char* argv[])
{
  const char* name = "Value tester";
  unit_test_log.set_threshold_level(log_messages);
  framework::master_test_suite().p_name.value = name;
  BOOST_TEST_MESSAGE("");
  BOOST_TEST_MESSAGE(name);
  BOOST_TEST_MESSAGE(std::string(std::strlen(name), '='));
  return NULL;
}

BOOST_AUTO_TEST_SUITE(Test_Value)

BOOST_AUTO_TEST_CASE(stream_output_1)
{
    std::ostringstream tmp;
    Value v;
    tmp << v;
    BOOST_CHECK_EQUAL(tmp.str(), std::string("nan"));
}

BOOST_AUTO_TEST_CASE(compare_with_none)
{
    None none;
    Value v1(none);
    Value v2 = 1.3;
    Value v3 = v1;

    BOOST_CHECK(v1 == v3);
    BOOST_CHECK(v1 != v2);
}

// In C++20 "value == None()" may be rewritten as "None() == value", which used the template
// Spine::None::operator== returning false for every type but None
BOOST_AUTO_TEST_CASE(compare_directly_with_none)
{
    Value missing = None();
    Value number = 1.3;
    Value text = std::string("x");

    BOOST_CHECK(missing == None());
    BOOST_CHECK(!(missing != None()));
    BOOST_CHECK(None() == missing);
    BOOST_CHECK(!(None() != missing));

    BOOST_CHECK(number != None());
    BOOST_CHECK(!(number == None()));
    BOOST_CHECK(None() != number);
    BOOST_CHECK(text != None());
    BOOST_CHECK(!(None() == text));
}

BOOST_AUTO_TEST_CASE(get_double)
{
  None none;
  Fmi::LocalDateTime ldt;
  SmartMet::Spine::LonLat lonlat(24.0, 60.0);
  Value v1 = none;
  Value v2 = 42;
  Value v3 = 3.14;
  Value v4 = ldt;
  Value v5 = lonlat;
  Value v6 = "42.25";

  //std::cout << v2.as_string() << std::endl;

  BOOST_CHECK_THROW(v1.as_double(), Fmi::Exception);
  BOOST_CHECK_EQUAL(v2.as_double(), 42.0);
  BOOST_CHECK_EQUAL(v3.as_double(), 3.14);
  BOOST_CHECK_THROW(v4.as_double(), Fmi::Exception);
  BOOST_CHECK_THROW(v5.as_double(), Fmi::Exception);
  BOOST_CHECK_EQUAL(v6.as_double(), 42.25);
}

BOOST_AUTO_TEST_CASE(get_int)
{
  None none;
  Fmi::TimeZonePtr tz("Europe/Helsinki");
  Fmi::DateTime dt = Fmi::DateTime::from_iso_extended_string("2024-07-15T00:00:00");
  Fmi::LocalDateTime ldt(dt, tz);
  SmartMet::Spine::LonLat lonlat(24.0, 60.0);
  Value v1 = none;
  Value v2 = 42;
  Value v3 = 1024.0;
  Value v4 = ldt;
  Value v5 = lonlat;
  Value v6 = "42";

  BOOST_CHECK_THROW(v1.as_int(), Fmi::Exception);
  BOOST_CHECK_EQUAL(v2.as_int(), 42);
  BOOST_CHECK_EQUAL(v3.as_int(), 1024);
  BOOST_CHECK_THROW(v4.as_int(), Fmi::Exception);
  BOOST_CHECK_THROW(v5.as_int(), Fmi::Exception);
  BOOST_CHECK_EQUAL(v6.as_int(), 42);
}

// Without noexcept moves a std::vector copies its elements when it grows, and copying the
// time series of an area query took gigabytes
static_assert(std::is_nothrow_move_constructible_v<Value>);
static_assert(std::is_nothrow_move_constructible_v<SmartMet::TimeSeries::TimedValue>);
static_assert(std::is_nothrow_move_constructible_v<SmartMet::TimeSeries::TimeSeries>);
static_assert(std::is_nothrow_move_constructible_v<SmartMet::TimeSeries::LonLatTimeSeries>);

BOOST_AUTO_TEST_CASE(move_time_series)
{
  using SmartMet::TimeSeries::LonLatTimeSeries;
  using SmartMet::TimeSeries::TimedValue;
  using SmartMet::TimeSeries::TimeSeries;

  const Fmi::LocalDateTime ldt(Fmi::DateTime(Fmi::Date(2026, 11, 2), Fmi::Hours(12)),
                               Fmi::TimeZonePtr::utc);
  TimeSeries ts;
  ts.push_back(TimedValue(ldt, std::string("a long string which does not fit in SSO")));
  ts.emplace_back(TimedValue(ldt, 1.5));
  const auto* data = ts.data();

  LonLatTimeSeries llts(SmartMet::Spine::LonLat(25, 60), std::move(ts));
  BOOST_CHECK(llts.timeseries.data() == data);  // moved, not copied
  BOOST_CHECK_EQUAL(llts.timeseries.size(), 2U);
  BOOST_CHECK_EQUAL(llts.timeseries[1].value.as_double(), 1.5);

  TimeSeries copy;
  copy = llts.timeseries;
  BOOST_CHECK_EQUAL(copy.size(), 2U);
  BOOST_CHECK(copy.data() != data);

  TimeSeries moved;
  moved = std::move(copy);
  BOOST_CHECK_EQUAL(moved.size(), 2U);
}

BOOST_AUTO_TEST_SUITE_END()
