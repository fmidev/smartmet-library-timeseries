// ======================================================================
/*!
 * \brief Regression tests for TimeSeriesGenerator
 */
// ======================================================================

#include "TimeSeriesInclude.h"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <macgyver/StringConversion.h>
#include <macgyver/TimeParser.h>
#include <macgyver/TimeZones.h>
#include <regression/tframe.h>

Fmi::TimeZones timezones;

std::string tostr(const TS::TimeSeriesGenerator::LocalTimeList& tlist)
{
  std::ostringstream out;
  BOOST_FOREACH (const Fmi::LocalDateTime& t, tlist)
  {
    out << t << '\n';
  }
  return out.str();
}

// Protection against namespace tests
namespace TimeSeriesGeneratorTest
{
// ----------------------------------------------------------------------
/*!
 * \brief Test the fixedtimes mode
 */
// ----------------------------------------------------------------------

void fixedtimes_utc()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::FixedTimes;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 11, 13), Fmi::Hours(5));
  opt.startTimeUTC = false;
  opt.timeSteps = 4;
  opt.timeList.insert(300);
  opt.timeList.insert(1300);
  opt.timeList.insert(1700);
  std::string result =
      "2012-Nov-13 13:00:00 UTC\n"
      "2012-Nov-13 17:00:00 UTC\n"
      "2012-Nov-14 03:00:00 UTC\n"
      "2012-Nov-14 13:00:00 UTC\n";

  auto tz = timezones.time_zone_from_string("UTC");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple UTC test:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void fixedtimes_helsinki()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::FixedTimes;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 11, 13), Fmi::Hours(5));
  opt.startTimeUTC = false;
  opt.timeSteps = 4;
  opt.timeList.insert(300);
  opt.timeList.insert(1300);
  opt.timeList.insert(1700);
  std::string result =
      "2012-Nov-13 13:00:00 EET\n"
      "2012-Nov-13 17:00:00 EET\n"
      "2012-Nov-14 03:00:00 EET\n"
      "2012-Nov-14 13:00:00 EET\n";

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple Europe/Helsinki test:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void fixedtimes_tosummertime()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::FixedTimes;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 3, 24), Fmi::Hours(0));
  opt.startTimeUTC = false;
  opt.timeSteps = 11;
  opt.timeList.insert(200);
  opt.timeList.insert(300);
  opt.timeList.insert(400);
  opt.timeList.insert(1300);
  std::string result =
      "2012-Mar-24 02:00:00 EET\n"
      "2012-Mar-24 03:00:00 EET\n"
      "2012-Mar-24 04:00:00 EET\n"
      "2012-Mar-24 13:00:00 EET\n"
      "2012-Mar-25 02:00:00 EET\n"
      "2012-Mar-25 04:00:00 EEST\n"
      "2012-Mar-25 13:00:00 EEST\n"
      "2012-Mar-26 02:00:00 EEST\n"
      "2012-Mar-26 03:00:00 EEST\n"
      "2012-Mar-26 04:00:00 EEST\n"
      "2012-Mar-26 13:00:00 EEST\n";

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple Europe/Helsinki test:\n" + ret + " <>\n " + result);

  TEST_PASSED();
}

void fixedtimes_towintertime()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::FixedTimes;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 10, 27), Fmi::Hours(0));
  opt.startTimeUTC = false;
  opt.timeSteps = 11;
  opt.timeList.insert(200);
  opt.timeList.insert(300);
  opt.timeList.insert(400);
  opt.timeList.insert(1300);
  std::string result =
      "2012-Oct-27 02:00:00 EEST\n"
      "2012-Oct-27 03:00:00 EEST\n"
      "2012-Oct-27 04:00:00 EEST\n"
      "2012-Oct-27 13:00:00 EEST\n"
      "2012-Oct-28 02:00:00 EEST\n"
      "2012-Oct-28 03:00:00 EEST\n"
      "2012-Oct-28 04:00:00 EET\n"
      "2012-Oct-28 13:00:00 EET\n"
      "2012-Oct-29 02:00:00 EET\n"
      "2012-Oct-29 03:00:00 EET\n"
      "2012-Oct-29 04:00:00 EET\n";

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple Europe/Helsinki test:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void fixedtimes_day()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::FixedTimes;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 11, 13), Fmi::Hours(5));
  opt.startTimeUTC = false;
  opt.timeSteps = 4;
  opt.timeList.insert(300);
  opt.timeList.insert(1300);
  opt.timeList.insert(1700);
  opt.days.insert(14);
  std::string result =
      "2012-Nov-14 03:00:00 UTC\n"
      "2012-Nov-14 13:00:00 UTC\n"
      "2012-Nov-14 17:00:00 UTC\n"
      "2012-Dec-14 03:00:00 UTC\n";

  auto tz = timezones.time_zone_from_string("UTC");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple UTC day test:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

// ----------------------------------------------------------------------
/*!
 * \brief Test the timesteps mode
 */
// ----------------------------------------------------------------------

void timesteps_utc()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 11, 13), Fmi::Hours(5));
  opt.startTimeUTC = false;
  opt.endTime = opt.startTime + Fmi::Hours(24);
  opt.timeStep = 180;
  opt.timeSteps = 4;
  std::string result =
      "2012-Nov-13 06:00:00 UTC\n"
      "2012-Nov-13 09:00:00 UTC\n"
      "2012-Nov-13 12:00:00 UTC\n"
      "2012-Nov-13 15:00:00 UTC\n";

  auto tz = timezones.time_zone_from_string("UTC");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple UTC test:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void timesteps_helsinki()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 11, 13), Fmi::Hours(5));
  opt.startTimeUTC = false;
  opt.endTime = opt.startTime + Fmi::Hours(24);
  opt.timeStep = 180;
  opt.timeSteps = 4;
  std::string result =
      "2012-Nov-13 06:00:00 EET\n"
      "2012-Nov-13 09:00:00 EET\n"
      "2012-Nov-13 12:00:00 EET\n"
      "2012-Nov-13 15:00:00 EET\n";

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple Europe/Helsinki test:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void timesteps_tosummertime()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 3, 25), Fmi::Hours(0));
  opt.startTimeUTC = false;
  opt.endTime = opt.startTime + Fmi::Hours(48);
  opt.timeSteps = 8;
  opt.timeStep = 60;
  std::string result =
      "2012-Mar-25 00:00:00 EET\n"
      "2012-Mar-25 01:00:00 EET\n"
      "2012-Mar-25 02:00:00 EET\n"
      "2012-Mar-25 04:00:00 EEST\n"
      "2012-Mar-25 05:00:00 EEST\n"
      "2012-Mar-25 06:00:00 EEST\n"
      "2012-Mar-25 07:00:00 EEST\n"
      "2012-Mar-25 08:00:00 EEST\n";

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple Europe/Helsinki test:\n" + ret + " <>\n " + result);

  TEST_PASSED();
}

void timesteps_towintertime()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 10, 28), Fmi::Hours(0));
  opt.startTimeUTC = false;
  opt.endTime = opt.startTime + Fmi::Hours(48);
  opt.timeSteps = 8;
  opt.timeStep = 60;
  std::string result =
      "2012-Oct-28 00:00:00 EEST\n"
      "2012-Oct-28 01:00:00 EEST\n"
      "2012-Oct-28 02:00:00 EEST\n"
      "2012-Oct-28 03:00:00 EEST\n"
      "2012-Oct-28 04:00:00 EET\n"
      "2012-Oct-28 05:00:00 EET\n"
      "2012-Oct-28 06:00:00 EET\n"
      "2012-Oct-28 07:00:00 EET\n";

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple Europe/Helsinki test:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void timesteps_all()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 11, 13), Fmi::Hours(5));
  opt.startTimeUTC = false;
  opt.endTime = opt.startTime + Fmi::Hours(24);
  opt.timeStep = 0;
  opt.timeSteps = 4;  // meaningless
  std::string result =
      "2012-Nov-13 05:00:00 UTC\n"
      "2012-Nov-14 05:00:00 UTC\n";

  auto tz = timezones.time_zone_from_string("UTC");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple UTC test:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void datatimes()
{
  using namespace SmartMet::TimeSeries;

  auto tlist = boost::make_shared<TimeSeriesGeneratorOptions::TimeList::element_type>();

  tlist->push_back(Fmi::DateTime(Fmi::Date(2012, 10, 28), Fmi::Hours(12)));
  tlist->push_back(Fmi::DateTime(Fmi::Date(2012, 10, 29), Fmi::Hours(12)));
  tlist->push_back(Fmi::DateTime(Fmi::Date(2012, 10, 30), Fmi::Hours(12)));
  tlist->push_back(Fmi::DateTime(Fmi::Date(2012, 10, 31), Fmi::Hours(12)));

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::DataTimes;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 10, 28), Fmi::Hours(0));
  opt.startTimeUTC = false;
  opt.setDataTimes(tlist, false);
  opt.timeSteps = 2;

  std::string result =
      "2012-Oct-28 14:00:00 EET\n"
      "2012-Oct-29 14:00:00 EET\n";

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to select datatimes:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void timesteps_day()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::DateTime(Fmi::Date(2012, 11, 13), Fmi::Hours(5));
  opt.startTimeUTC = false;
  opt.endTime = opt.startTime + Fmi::Hours(48);
  opt.timeStep = 180;
  opt.timeSteps = 4;
  opt.days.insert(14);
  std::string result =
      "2012-Nov-14 00:00:00 UTC\n"
      "2012-Nov-14 03:00:00 UTC\n"
      "2012-Nov-14 06:00:00 UTC\n"
      "2012-Nov-14 09:00:00 UTC\n";

  auto tz = timezones.time_zone_from_string("UTC");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to pass a simple UTC day test:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void datatimes_climatology()
{
  using namespace SmartMet::TimeSeries;

  auto tlist = boost::make_shared<TimeSeriesGeneratorOptions::TimeList::element_type>();

  tlist->push_back(Fmi::DateTime(Fmi::Date(2012, 10, 28), Fmi::Hours(12)));
  tlist->push_back(Fmi::DateTime(Fmi::Date(2012, 10, 29), Fmi::Hours(12)));
  tlist->push_back(Fmi::DateTime(Fmi::Date(2012, 10, 30), Fmi::Hours(12)));
  tlist->push_back(Fmi::DateTime(Fmi::Date(2012, 10, 31), Fmi::Hours(12)));

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::DataTimes;
  opt.startTime = Fmi::DateTime(Fmi::Date(2011, 10, 29), Fmi::Hours(0));
  opt.startTimeUTC = false;
  opt.endTime = Fmi::DateTime(Fmi::Date(2013, 10, 29), Fmi::Hours(0));
  opt.endTimeUTC = false;
  opt.setDataTimes(tlist, true);

  std::string result =
      "2011-Oct-29 15:00:00 EEST\n"
      "2011-Oct-30 14:00:00 EET\n"
      "2011-Oct-31 14:00:00 EET\n"
      "2012-Oct-28 14:00:00 EET\n"
      "2012-Oct-29 14:00:00 EET\n"
      "2012-Oct-30 14:00:00 EET\n"
      "2012-Oct-31 14:00:00 EET\n"
      "2013-Oct-28 14:00:00 EET\n";

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");
  std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
  if (ret != result)
    TEST_FAILED("Failed to select datatimes:\n" + ret + " <>\n" + result);

  TEST_PASSED();
}

void epochtime()
{
  using namespace SmartMet::TimeSeries;

  std::string starttime = "1454803200";

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::TimeParser::parse(starttime);
  opt.startTimeUTC = Fmi::TimeParser::looks_utc(starttime);
  opt.endTime = opt.startTime + Fmi::Hours(24);
  opt.timeStep = 60;
  opt.timeSteps = 4;

  // $ date -d@1454803200
  // Sun Feb  7 02:00:00 EET 2016

  {
    std::string result =
        "2016-Feb-07 02:00:00 EET\n"
        "2016-Feb-07 03:00:00 EET\n"
        "2016-Feb-07 04:00:00 EET\n"
        "2016-Feb-07 05:00:00 EET\n";

    auto tz = timezones.time_zone_from_string("Europe/Helsinki");
    std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
    if (ret != result)
      TEST_FAILED("Failed to generate the correct times with tz=Europe/Helsinki:\n" + ret +
                  " <>\n" + result);
  }

  {
    std::string result =
        "2016-Feb-07 00:00:00 UTC\n"
        "2016-Feb-07 01:00:00 UTC\n"
        "2016-Feb-07 02:00:00 UTC\n"
        "2016-Feb-07 03:00:00 UTC\n";

    auto tz = timezones.time_zone_from_string("UTC");
    std::string ret = tostr(TimeSeriesGenerator::generate(opt, tz));
    if (ret != result)
      TEST_FAILED("Failed to generate the correct times with tz=UTC:\n" + ret + " <>\n" + result);
  }

  TEST_PASSED();
}

void offset()
{
  using namespace SmartMet::TimeSeries;

  std::string starttime = "-1h";

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::TimeParser::parse(starttime);
  opt.startTimeUTC = Fmi::TimeParser::looks_utc(starttime);
  opt.endTime = opt.startTime + Fmi::Hours(24);
  opt.timeStep = 60;
  opt.timeSteps = 2;

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");
  auto series = TimeSeriesGenerator::generate(opt, tz);
  if (series.size() != 2)
    TEST_FAILED("Expected two times in the result");

  auto diff = Fmi::SecondClock::universal_time() - series.front().utc_time();

  // We expect to get the current time rounded down to the exact hour

  if (diff < Fmi::Minutes(0) || diff > Fmi::Minutes(60))
    TEST_FAILED("Too large time difference to current time: " + Fmi::to_simple_string(diff));

  TEST_PASSED();
}

namespace
{
  struct ObsDefaults : public SmartMet::TimeSeries::TimeSeriesGeneratorOptions::Defaults
  {
    constexpr ObsDefaults()
    {
      defaultStartOffset = -1440;
    }
  };

  constexpr ObsDefaults obs_defaults;
}

void parse_http_request_empty()
{
  using namespace SmartMet::Spine;
  using namespace SmartMet::TimeSeries;

  const Fmi::DateTime now = Fmi::DateTime::from_iso_extended_string("2012-11-13T05:00:00Z");
  const Fmi::DateTime expected_start = now - Fmi::Hours(24);
  const Fmi::DateTime expected_end = now;

  HTTP::Request req;
  TimeSeriesGeneratorOptions opt = TimeSeriesGeneratorOptions::parse(req, obs_defaults, now);

  if (opt.mode != TimeSeriesGeneratorOptions::Mode::TimeSteps)
    TEST_FAILED("Mode::Timesteps expected");

  if (!opt.startTimeAssumed)
    TEST_FAILED("Assumed start time expected");

  if (!opt.startTimeUTC)
    TEST_FAILED("UTC start time expected");

  if (opt.startTime != expected_start)
    TEST_FAILED("Start time " + expected_start.to_simple_string() + " expected. Got "
        + opt.startTime.to_simple_string());

  if (opt.endTime != expected_end)
    TEST_FAILED("End time " + expected_end.to_simple_string() + " expected. Got "
        + expected_end.to_simple_string());

  TEST_PASSED();
}

void parse_http_request_1()
{
  using namespace SmartMet::Spine;
  using namespace SmartMet::TimeSeries;

  HTTP::Request req;
  req.setParameter("starttime", "2012-11-13T05:00:00Z");
  req.setParameter("endtime", "2012-11-14T05:00:00Z");
  req.setParameter("timestep", "180");

  TimeSeriesGeneratorOptions opt = TimeSeriesGeneratorOptions::parse(req);

  if (opt.mode != TimeSeriesGeneratorOptions::Mode::TimeSteps)
    TEST_FAILED("Failed to parse mode");

  if (opt.startTime != Fmi::TimeParser::parse("2012-11-13T05:00:00Z"))
    TEST_FAILED("Failed to parse starttime");

  if (opt.endTime != Fmi::TimeParser::parse("2012-11-14T05:00:00Z"))
    TEST_FAILED("Failed to parse endtime");

  if (!opt.timeStep || *opt.timeStep != 180)
    TEST_FAILED("Failed to parse timestep");

  TEST_PASSED();
}

// ----------------------------------------------------------------------
/*!
 * The actual test suite
 */
// ----------------------------------------------------------------------

class tests : public tframe::tests
{
  virtual const char* error_message_prefix() const { return "\n\t"; }
  void test()
  {
    TEST(fixedtimes_utc);
    TEST(fixedtimes_helsinki);
    TEST(fixedtimes_towintertime);
    TEST(fixedtimes_tosummertime);
    TEST(fixedtimes_day);
    TEST(timesteps_utc);
    TEST(timesteps_helsinki);
    TEST(timesteps_tosummertime);
    TEST(timesteps_towintertime);
    TEST(timesteps_all);
    TEST(timesteps_day);
    TEST(epochtime);
    TEST(offset);
    TEST(datatimes);
    TEST(datatimes_climatology);

    TEST(parse_http_request_empty);
    TEST(parse_http_request_1);
  }
};

}  // namespace TimeSeriesGeneratorTest

//! The main program
int main()
{
  using namespace std;
  cout << endl << "TimeSeriesGenerator tester" << endl << "==========================" << endl;
  TimeSeriesGeneratorTest::tests t;
  return t.run();
}
