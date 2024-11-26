#include "ParameterTools.h"
#include <boost/test/included/unit_test.hpp>
#include <sstream>

using namespace SmartMet;
using namespace boost::unit_test;

namespace TS = TimeSeries;

test_suite* init_unit_test_suite(int argc, char* argv[])
{
  const char* name = "ParameterTools tester";
  unit_test_log.set_threshold_level(log_messages);
  framework::master_test_suite().p_name.value = name;
  BOOST_TEST_MESSAGE("");
  BOOST_TEST_MESSAGE(name);
  BOOST_TEST_MESSAGE(std::string(std::strlen(name), '='));
  return NULL;
}

BOOST_AUTO_TEST_SUITE(Test_ParameterTools)

BOOST_AUTO_TEST_CASE(check_some_parameters)
{
    Spine::Parameter p1 = TS::makeParameter("temperature");
    Spine::Parameter p2 = TS::makeParameter("fmisid");

    BOOST_CHECK(!TS::is_data_independent(p1));
    BOOST_CHECK(TS::is_data_independent(p2));
}

BOOST_AUTO_TEST_CASE(check_data_independent_parameters)
{
    // ----------------------------------------------------------------------
    // Does not actually have any assertions, just prints out the results
    // Therefore include the following to make boost happy
    BOOST_CHECK(true);
    // ----------------------------------------------------------------------

    int total = 0;
    int num_data_independent = 0;
    int location = 0;
    int time = 0;
    std::vector<std::string> other;

    std::vector<std::string> special = TS::special_parameters();
    for (const auto& s : special)
    {
        total++;
        Spine::Parameter p = TS::makeParameter(s);
        if (TS::is_data_independent(p))
        {
            num_data_independent++;
            if (TS::is_location_parameter(s))
                location++;
            else if (TS::is_time_parameter(s))
                time++;
            else
                other.push_back(s);
        }
    }

    std::cout << "\n";
    std::cout << "Total number of special parameters: " << total << std::endl;
    std::cout << "Number of data independent parameters: " << num_data_independent << std::endl;
    std::cout << "Number of location parameters: " << location << std::endl;
    std::cout << "Number of time parameters: " << time << std::endl;
    std::cout << "Other data independent parameters: " << std::endl;
    for (const auto& s : other)
    {
        std::cout << "  " << s << std::endl;
    }
}

BOOST_AUTO_TEST_SUITE_END()
