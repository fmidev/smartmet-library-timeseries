#include "LocationParameters.h"
#include "TimeSeriesOutput.h"
#include <iostream>
#include <sstream>
#include <macgyver/ValueFormatter.h>
#include <spine/None.h>
#include <boost/test/included/unit_test.hpp>

using namespace SmartMet::Spine;
using namespace SmartMet::TimeSeries::SpecialParameters;
namespace TS = SmartMet::TimeSeries;

using namespace boost::unit_test;

test_suite* init_unit_test_suite(int argc, char* argv[])
{
  const char* name = "LocationParameters tester";
  // unit_test_log.set_threshold_level(log_test_units);
  unit_test_log.set_threshold_level(log_messages);
  framework::master_test_suite().p_name.value = name;
  BOOST_TEST_MESSAGE("");
  BOOST_TEST_MESSAGE(name);
  BOOST_TEST_MESSAGE(std::string(std::strlen(name), '='));
  return nullptr;
}

namespace
{
    SmartMet::Spine::Location loc(
        843429, "Kumpula", "fi", 20, "Helsinki", "PPLX", "Suomi", 24.96420, 60.20890, "EEST", 3600, 11, -1);
    Fmi::ValueFormatterParam vfparam;
    Fmi::ValueFormatter vf(vfparam);
    const std::string tz = "Europe/Helsinki";
    int precision = 6;
    std::string crs = "EPSG:4326";

    std::string value2str(const TS::Value& value)
    {
        std::ostringstream os;
        Fmi::ValueFormatterParam vfparam;
        Fmi::ValueFormatter vf(vfparam);
        TS::OStreamVisitor visitor(os, vf, 6);
        std::visit(visitor, dynamic_cast<const TS::Value_&>(value));
        return os.str();
    }
}

BOOST_AUTO_TEST_CASE(test_1)
{
    // Got test values by querying https://opendata.fmi.fi/timeseries
    struct
    {
        const char* id;
        const char* expected;
    }
    tests[] = {
        { "country", "Suomi" },
        { "latlon", "60.208900, 24.964200" },
        { "lonlat", "24.964200, 60.208900" },
        { "name", "Kumpula" },
        { "population", "3600" },
        { "region", "Helsinki" },
        { "elevation", "11.000000" },
        { "fmisid", nullptr},
        { "feature", "PPLX"}
    };

    int num_tests = 0;
    int num_errors = 0;

    for (const auto& test : tests)
    {
        BOOST_TEST_MESSAGE("+ [" + std::string(test.id) + "]");
        LocationParameterArgs args(loc, vf, tz, crs);

        TS::Value ret;

        ret = LocationParameters::instance(test.id, args, precision);

        if (test.expected)
        {
            if (std::holds_alternative<None>(ret))
            {
                BOOST_TEST_ERROR("  - Expected data, got Spine::None");
                ++num_errors;
            }
            else
            {
                const std::string result_str = value2str(ret);
                if (result_str != std::string(test.expected))
                {
                    BOOST_TEST_ERROR("  - Expected '"
                        + std::string(test.expected) + "', got '"
                                + result_str + "'");
                    ++num_errors;
                }
            }
        }
        else
        {
            if (!std::holds_alternative<None>(ret))
            {
                BOOST_TEST_ERROR("  - Expected empty, got something else: "
                              + value2str(ret));
                ++num_errors;
            }
        }
    }

    if (num_errors)
    {
        std::cout << "Errors in " << num_errors << " out of " << num_tests << " tests";
    }

    // Silence boost::test warning about no tests
    BOOST_CHECK(true);
}
