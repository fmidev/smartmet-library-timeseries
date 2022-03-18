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

BOOST_AUTO_TEST_SUITE_END()

