#include "ParameterFactory.h"
#include <boost/test/included/unit_test.hpp>
#include <macgyver/Exception.h>
#include <limits>

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

BOOST_AUTO_TEST_CASE(colons_in_parameter_names)
{
  const auto& factory = TS::ParameterFactory::instance();
  // Intervals not given in the request are left at the maximum value
  const auto unset = std::numeric_limits<unsigned int>::max();

  // Grid parameter names contain colons, which must not be taken for intervals. Like the
  // plugins, accept parameter names unknown to the factory.
  auto pf = factory.parseNameAndFunctions("nanmean_t(T-K:MEPS:1093:6:2:4:0)", true);
  BOOST_CHECK_EQUAL(pf.parameter.name(), "t-k:meps:1093:6:2:4:0");
  BOOST_CHECK(pf.functions.innerFunction.type() == TS::FunctionType::TimeFunction);
  BOOST_CHECK_EQUAL(pf.functions.innerFunction.getAggregationIntervalBehind(), unset);
  BOOST_CHECK_EQUAL(pf.functions.innerFunction.getAggregationIntervalAhead(), unset);

  pf = factory.parseNameAndFunctions("nanmean_t(nanmean(T-K:MEPS:1093:6:2:4:0))", true);
  BOOST_CHECK_EQUAL(pf.parameter.name(), "t-k:meps:1093:6:2:4:0");
  BOOST_CHECK(pf.functions.outerFunction.type() == TS::FunctionType::TimeFunction);
  BOOST_CHECK_EQUAL(pf.functions.outerFunction.getAggregationIntervalBehind(), unset);

  // Intervals given with slashes
  pf = factory.parseNameAndFunctions("nanmean_t(T-K:MEPS:1093:6:2:4:0/1h)", true);
  BOOST_CHECK_EQUAL(pf.parameter.name(), "t-k:meps:1093:6:2:4:0");
  BOOST_CHECK_EQUAL(pf.functions.innerFunction.getAggregationIntervalBehind(), 60U);

  pf = factory.parseNameAndFunctions("nanmean_t(nanmean(T-K:MEPS:1093:6:2:4:0/0m/60m))", true);
  BOOST_CHECK_EQUAL(pf.parameter.name(), "t-k:meps:1093:6:2:4:0");
  BOOST_CHECK(pf.functions.innerFunction.type() == TS::FunctionType::AreaFunction);
  BOOST_CHECK_EQUAL(pf.functions.outerFunction.getAggregationIntervalBehind(), 0U);
  BOOST_CHECK_EQUAL(pf.functions.outerFunction.getAggregationIntervalAhead(), 60U);

  // The interval belongs next to the parameter name, not after the inner function
  BOOST_CHECK_THROW(
      factory.parseNameAndFunctions("nanmean_t(nanmean(T-K:MEPS:1093:6:2:4:0)/0m/60m)", true),
      Fmi::Exception);

  // The legacy colon syntax for intervals still works
  pf = factory.parseNameAndFunctions("mean_t(Temperature:1h)");
  BOOST_CHECK_EQUAL(pf.parameter.name(), "Temperature");
  BOOST_CHECK_EQUAL(pf.functions.innerFunction.getAggregationIntervalBehind(), 60U);
  BOOST_CHECK_EQUAL(pf.functions.innerFunction.getAggregationIntervalAhead(), unset);

  pf = factory.parseNameAndFunctions("mean_t(Temperature:30m:1h)");
  BOOST_CHECK_EQUAL(pf.functions.innerFunction.getAggregationIntervalBehind(), 30U);
  BOOST_CHECK_EQUAL(pf.functions.innerFunction.getAggregationIntervalAhead(), 60U);

  BOOST_CHECK_THROW(factory.parseNameAndFunctions("mean_t(Temperature:-1h)"), Fmi::Exception);
}

BOOST_AUTO_TEST_SUITE_END()
