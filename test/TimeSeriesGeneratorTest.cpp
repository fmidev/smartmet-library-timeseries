// ======================================================================
/*!
 * \brief Regression tests for TimeSeriesGenerator
 */
// ======================================================================

#include "TimeSeriesInclude.h"
#include <boost/make_shared.hpp>
#include <macgyver/StringConversion.h>
#include <macgyver/TimeParser.h>
#include <macgyver/TimeZones.h>
#include <regression/tframe.h>

Fmi::TimeZones timezones;

std::string tostr(const TS::TimeSeriesGenerator::LocalTimeList& tlist)
{
  std::ostringstream out;
  for (const Fmi::LocalDateTime& t : tlist)
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

void excessive_timesteps()
{
  using namespace SmartMet::TimeSeries;

  // A one-minute step over a multi-decade span would build a set of tens of millions of
  // times and OOM/hang the process. The generator must instead throw once it exceeds its
  // hard safety limit rather than attempt the allocation.
  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::DateTime(Fmi::Date(2000, 1, 1), Fmi::Hours(0));
  opt.startTimeUTC = true;
  opt.endTime = opt.startTime + Fmi::Hours(24 * 365 * 40);  // ~40 years
  opt.timeStep = 1;                                         // one minute

  auto tz = timezones.time_zone_from_string("UTC");
  try
  {
    (void)TimeSeriesGenerator::generate(opt, tz);
    TEST_FAILED("Unbounded time-step generation should have thrown");
  }
  catch (...)
  {
    // Expected: the hard safety limit was hit.
  }
  TEST_PASSED();
}

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

  auto tlist = std::make_shared<TimeSeriesGeneratorOptions::TimeList::element_type>();

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

  auto tlist = std::make_shared<TimeSeriesGeneratorOptions::TimeList::element_type>();

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
  // Earlier version used offset "-1h" with Fmi::TimeParser. Fmi::TimeParser testing
  // is done elsewhere, here we just test that TimeSeriesGenerator generated times
  // are correctly offsetted.
  using namespace SmartMet::TimeSeries;

  int numErrors = 0;
  constexpr const int MAX_ERRORS = 10;

  auto tz = timezones.time_zone_from_string("Europe/Helsinki");

  // A fixed day without a daylight saving time change. With the current day the test failed
  // on the day before the change to winter time: the hourly steps are in local time, so the
  // repeated hour is skipped (see timesteps_towintertime) and the second time is two hours
  // after the first in UTC.
  const Fmi::DateTime start = Fmi::DateTime(Fmi::Date(2012, 11, 13), Fmi::Seconds(0));

  for (unsigned seconds = 0; seconds < 86400 && numErrors < MAX_ERRORS; seconds += 1)
  {
    std::ostringstream msg;

    bool isError = false;
    // Expect full hours. Take rounding down to full minutes into account
    // (TimeSeriesGenerator ignores seconds and takes only full minutes into account)
    const unsigned tmp = (seconds + 59) / 60;
    const Fmi::DateTime next_full_hour = start + Fmi::Hours((59 + tmp) / 60);
    //
    TimeSeriesGeneratorOptions opt;
    opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
    opt.startTime = start + Fmi::Seconds(seconds);
    opt.startTimeUTC = true;
    opt.endTime = opt.startTime + Fmi::Hours(24);
    opt.timeStep = 60;
    opt.timeSteps = 2;

    auto series = TimeSeriesGenerator::generate(opt, tz);

    msg << "Start time: " << opt.startTime << "\n";
    msg << "Next full hour: " << Fmi::LocalDateTime(next_full_hour, tz) << "\n";
    msg << "Generated times:\n";
    for (const auto& t : series)
      msg << "  " << t << "\n";

    if (series.size() != 2)
    {
      isError = true;
      msg << "  Expected two times in the result\n";
    }

    // We expect to get the start time and start time + 1h

    auto diff = next_full_hour - series.front().utc_time();
    if (diff != Fmi::Seconds(0))
    {
      isError = true;
      msg << "  Too large time difference to start time: " << Fmi::to_simple_string(diff) << "\n";
    }

    diff = series.back().utc_time() - (next_full_hour + Fmi::Hours(1));
    if (diff != Fmi::Seconds(0))
    {
      isError = true;
      msg << "  Too large time difference to start time + 1h: " << Fmi::to_simple_string(diff) << "\n";
    }

    if (isError)
    {
      numErrors++;
      std::cout << '\n' << msg.str();
      msg.str("");
    }
  }

  if (numErrors > 0)
  {
    if (numErrors >= MAX_ERRORS)
    {
      TEST_FAILED("Maximum number of errors (" + Fmi::to_string(MAX_ERRORS) + ") reached, stopping test");
    }
    else
    {
      TEST_FAILED("Encountered " + Fmi::to_string(numErrors) + " exceptions in offset test");
    }
  }
  TEST_PASSED();
}

// Return true if the function throws. TEST_FAILED throws too, so it must not be called
// inside a try block with a catch-all handler.
template <typename F>
bool throws(F&& f)
{
  try
  {
    f();
  }
  catch (...)
  {
    return true;
  }
  return false;
}

// ----------------------------------------------------------------------
/*!
 * \brief Test that fixed times are limited by the number of generated times, not days
 */
// ----------------------------------------------------------------------

void excessive_fixedtimes()
{
  using namespace SmartMet::TimeSeries;

  // 24 times per day over ~1000 years is ~8.8M days * 24 = 210M times, which used to pass
  // the hard limit since it counted days only.
  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::FixedTimes;
  opt.startTime = Fmi::DateTime(Fmi::Date(2000, 1, 1), Fmi::Hours(0));
  opt.startTimeUTC = true;
  opt.endTime = Fmi::DateTime(Fmi::Date(3000, 1, 1), Fmi::Hours(0));
  for (unsigned int h = 0; h < 24; h++)
    opt.timeList.insert(h * 100);

  auto tz = timezones.time_zone_from_string("UTC");

  // A caller limit applies to the generated times
  if (!throws([&] { (void)TimeSeriesGenerator::generate(opt, tz, 1000); }))
    TEST_FAILED("Fixed-times generation should have thrown with maxtimes=1000");

  // A shorter request within the limit works as before
  opt.endTime = Fmi::DateTime(Fmi::Date(2000, 1, 2), Fmi::Hours(0));
  auto tlist = TimeSeriesGenerator::generate(opt, tz, 25);
  if (tlist.size() != 25)
    TEST_FAILED("Expected 25 times, got " + std::to_string(tlist.size()));
  if (!throws([&] { (void)TimeSeriesGenerator::generate(opt, tz, 24); }))
    TEST_FAILED("Fixed-times generation should have thrown with maxtimes=24");

  TEST_PASSED();
}

// ----------------------------------------------------------------------
/*!
 * \brief Test the caller limit in timesteps mode
 */
// ----------------------------------------------------------------------

void maxtimes_timesteps()
{
  using namespace SmartMet::TimeSeries;

  TimeSeriesGeneratorOptions opt;
  opt.mode = TimeSeriesGeneratorOptions::Mode::TimeSteps;
  opt.startTime = Fmi::DateTime(Fmi::Date(2000, 1, 1), Fmi::Hours(0));
  opt.startTimeUTC = true;
  opt.endTime = opt.startTime + Fmi::Hours(99);  // 100 hourly steps
  opt.timeStep = 60;

  auto tz = timezones.time_zone_from_string("UTC");

  if (TimeSeriesGenerator::generate(opt, tz, 100).size() != 100)
    TEST_FAILED("Expected 100 timesteps with maxtimes=100");
  if (TimeSeriesGenerator::generate(opt, tz, 0).size() != 100)
    TEST_FAILED("Expected 100 timesteps with maxtimes=0");

  if (!throws([&] { (void)TimeSeriesGenerator::generate(opt, tz, 99); }))
    TEST_FAILED("Timestep generation should have thrown with maxtimes=99");

  // The cache must apply the limit to cached lists too
  TimeSeriesGeneratorCache cache;
  cache.resize(10);
  if (cache.generate(opt, tz, 100)->size() != 100)
    TEST_FAILED("Expected 100 timesteps from the cache");
  if (!throws([&] { (void)cache.generate(opt, tz, 99); }))
    TEST_FAILED("Cached timestep list should have thrown with maxtimes=99");

  TEST_PASSED();
}

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
    TEST(excessive_timesteps);
    TEST(excessive_fixedtimes);
    TEST(maxtimes_timesteps);
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
