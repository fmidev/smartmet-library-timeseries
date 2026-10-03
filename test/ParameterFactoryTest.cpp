#include "ParameterFactory.h"
#include <boost/test/included/unit_test.hpp>
#include <macgyver/Exception.h>

using namespace SmartMet;
using namespace boost::unit_test;

namespace TS = TimeSeries;

test_suite* init_unit_test_suite(int argc, char* argv[])
{
  const char* name = "ParameterFactory tester";
  unit_test_log.set_threshold_level(log_messages);
  framework::master_test_suite().p_name.value = name;
  BOOST_TEST_MESSAGE("");
  BOOST_TEST_MESSAGE(name);
  BOOST_TEST_MESSAGE(std::string(std::strlen(name), '='));
  return NULL;
}

BOOST_AUTO_TEST_SUITE(Test_ParameterFactory)

BOOST_AUTO_TEST_CASE(nested_functions)
{
  const auto& factory = TS::ParameterFactory::instance();

  // One time function and one area function in either order
  auto pf = factory.parseNameAndFunctions("mean_t(max(temperature/3h))");
  BOOST_CHECK(pf.functions.innerFunction.type() == TS::FunctionType::AreaFunction);
  BOOST_CHECK(pf.functions.outerFunction.type() == TS::FunctionType::TimeFunction);

  pf = factory.parseNameAndFunctions("max(mean_t(temperature/3h))");
  BOOST_CHECK(pf.functions.innerFunction.type() == TS::FunctionType::TimeFunction);
  BOOST_CHECK(pf.functions.outerFunction.type() == TS::FunctionType::AreaFunction);

  // Two functions of the same kind cannot both be applied
  BOOST_CHECK_THROW(factory.parseNameAndFunctions("max_t(mean_t(temperature/3h))"),
                    Fmi::Exception);
  BOOST_CHECK_THROW(factory.parseNameAndFunctions("max(mean(temperature))"), Fmi::Exception);
}

BOOST_AUTO_TEST_SUITE_END()
