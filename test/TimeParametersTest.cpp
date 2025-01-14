#include "TimeParameters.h"
#include "TimeSeriesOutput.h"
#include <iostream>
#include <sstream>
#include <macgyver/ValueFormatter.h>
#include <boost/test/included/unit_test.hpp>

using namespace SmartMet::Spine;
using namespace SmartMet::TimeSeries::SpecialParameters;
namespace TS = SmartMet::TimeSeries;

using namespace boost::unit_test;

test_suite* init_unit_test_suite(int argc, char* argv[])
{
  const char* name = "TimeParameters tester";
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
    const std::string tz_name = "Europe/Helsinki";
    Fmi::TimeZonePtr tz(tz_name);
    Fmi::Date date(2024, 12, 1);
    Fmi::DateTime dt(date, Fmi::TimeDuration(12, 0, 0));
    Fmi::DateTime now(date, Fmi::TimeDuration(9, 0, 0));
    Fmi::LocalDateTime ldt(dt, tz);
    SmartMet::Spine::Location loc1(
        843429, "Kumpula", "fi", 20, "Area", "Feature", "FI", 24.96420, 60.20890, tz_name, 500000, 50, -1);
    SmartMet::Spine::Location loc2(
        633268, "Utsjoki", "fi", 20, "Area", "Feature", "FI", 27.02843, 69.90864, tz_name, 500000, 50, -1);
    std::locale outlocale("fi_FI.utf8");
    std::unique_ptr<Fmi::TimeFormatter> timeformatter(Fmi::TimeFormatter::create("iso"));
    const std::string timeformat = "%Y-%m-%d %H:%M:%S %Z";

    Fmi::TimeZones timezones;

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

BOOST_AUTO_TEST_CASE(test_params)
{
    // Got test values by querying https://opendata.fmi.fi/timeseries
    struct
    {
        const char* id;
        const char* expected1;
        const char* expected2;
    }
    tests[] = {
        {"dark", "0", "1"},
        {"daylength", "383", "0"},
        {"epochtime", "1733054400", "1733054400"},
        {"hour", "14", "14"},
        {"isotime", "20241201T140000", "20241201T140000"},
        {"localtime", "20241201T140000", "20241201T140000"},
        {"mon", "joulu", "joulu"},
        {"month", "joulukuu", "joulukuu"},
        {"moondown24h", "0", "1"},
        {"moonphase", "0.722416", "0.722416"},
        {"moonrise", "20241201T101600", nullptr},
        {"moonrise2", nullptr, nullptr},
        {"moonrise2today", "0", "0"},
        {"moonrisetoday", "1", "0"},
        {"moonset", "20241201T140500", nullptr},
        {"moonset2", nullptr, nullptr},
        {"moonset2today", "0", "0"},
        {"moonsettoday", "1", "0"},
        {"moonup24h", "0", "0"},
        {"moondown24h", "0", "1"},
        {"noon", "20241201T120908", "20241201T120052"},
        {"origintime", "2024-12-01T11:00:00", "2024-12-01T11:00:00"},
        {"sundown24h", "0", "1"},
        {"sunup24h", "0", "0"},
        {"sunrise", "20241201T085735", "20250117T114238"},
        {"sunrisetoday", "1", "0"},
        {"sunset", "20241201T152037", "20241125T121441"},
        {"sunsettoday", "1", "0"},
        {"time", "20241201T140000", "20241201T140000"},
        {"tz", "Europe/Helsinki", "Europe/Helsinki"},
        {"utctime", "20241201T120000", "20241201T120000"},
        {"weekday", "sunnuntai", "sunnuntai"},
        {"xmltime", "2024-12-01T14:00:00", "2024-12-01T14:00:00"},
        {"date(%Y-%m-%d %H:%M:%S%Oz)", "2024-12-01 14:00:00,000000+02:00", "2024-12-01 14:00:00,000000+02:00"},
        {"date(%Y-%m-%d)", "2024-12-01", "2024-12-01"},
        {"date(%H:%M:%S)", "14:00:00,000000", "14:00:00,000000"}
    };

    int num_tests = 0;
    int num_errors = 0;
    for (const auto& test : tests)
    {
        BOOST_TEST_MESSAGE("+ [" + std::string(test.id) + "]");
        TimeParameterArgs args1(ldt, now, loc1, tz_name, timezones, outlocale, *timeformatter, timeformat);
        TimeParameterArgs args2(ldt, now, loc2, tz_name, timezones, outlocale, *timeformatter, timeformat);

        TS::Value ret;

        for (int i = 0; i < 2; i++)
        {
            TimeParameterArgs& args = i ? args2 : args1;
            const std::string name = i ? "Utsjoki" : "Kumpula";
            const char* expected = i ? test.expected2 : test.expected1;

            ret = TimeParameters::instance(test.id, args);

            if (expected)
            {
                if (std::holds_alternative<None>(ret))
                {
                    BOOST_TEST_ERROR("  - (" << name << ") Expected data, got Spine::None");
                    ++num_errors;
                }
                else
                {
                    const std::string result_str = value2str(ret);
                    if (result_str != std::string(expected))
                    {
                        BOOST_TEST_ERROR("  - (" << name << ") Expected '"
                            + std::string(expected) + "', got '"
                            + result_str + "'");
                        ++num_errors;
                    }
                }
            }
            else
            {
                if (!std::holds_alternative<None>(ret))
                {
                    BOOST_TEST_ERROR("  - (" << name << ") Expected empty, got something else: "
                            + value2str(ret));
                    ++num_errors;
                }
            }
        }
    }

    if (num_errors)
    {
        std::cout << "Errors in " << num_errors << " out of " << num_tests << " tests";
    }
    // Silence boost::test warning about no tests
}
