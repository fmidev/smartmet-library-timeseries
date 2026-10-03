// ======================================================================
/*!
 * \file
 * \brief Regression tests for parsing the time series generator options
 */
// ======================================================================

#include "TimeSeriesGeneratorOptions.h"
#include <macgyver/DateTime.h>
#include <regression/tframe.h>
#include <spine/HTTP.h>
#include <map>
#include <string>

using namespace std;
using SmartMet::TimeSeries::TimeSeriesGeneratorOptions;

namespace TimeSeriesGeneratorOptionsTest
{
// A fixed "now" so that the defaults are reproducible
const std::string now = "200808051234";

TimeSeriesGeneratorOptions parse(const std::map<std::string, std::string>& theOptions)
{
  SmartMet::Spine::HTTP::Request request;
  request.addParameter("now", now);
  for (const auto& option : theOptions)
    request.addParameter(option.first, option.second);
  return SmartMet::TimeSeries::parseTimes(request);
}

bool fails(const std::map<std::string, std::string>& theOptions)
{
  try
  {
    parse(theOptions);
    return false;
  }
  catch (...)
  {
    return true;
  }
}

Fmi::DateTime t(int day, int hour, int minute = 0)
{
  return {Fmi::Date(2008, 8, day), Fmi::Hours(hour) + Fmi::Minutes(minute)};
}

std::string str(const Fmi::DateTime& time)
{
  return Fmi::date_time::to_iso_string(time);
}

// ----------------------------------------------------------------------

void defaults()
{
  auto opt = parse({});
  if (opt.mode != TimeSeriesGeneratorOptions::TimeSteps)
    TEST_FAILED("Default mode should be TimeSteps");
  if (opt.startTime != t(5, 12, 34))
    TEST_FAILED("Default start time should be now, got " + str(opt.startTime));
  if (opt.endTime != t(6, 12, 34))
    TEST_FAILED("Default end time should be 24 hours after the start, got " + str(opt.endTime));
  if (!opt.startTimeUTC || !opt.endTimeUTC)
    TEST_FAILED("Times generated from now should be in UTC");
  TEST_PASSED();
}

// ----------------------------------------------------------------------

void timesteps()
{
  auto opt = parse({{"starttime", "200808051200"}, {"timestep", "30"}, {"timesteps", "5"}});
  if (!opt.timeStep || *opt.timeStep != 30)
    TEST_FAILED("Timestep should be 30");
  if (opt.endTime != t(5, 14, 30))
    TEST_FAILED("End time should be 5 steps of 30 minutes later, got " + str(opt.endTime));
  if (opt.startTimeUTC)
    TEST_FAILED("A start time without a zone is in local time");

  // The default timestep is one hour
  opt = parse({{"starttime", "200808051200"}, {"timesteps", "3"}});
  if (opt.endTime != t(5, 15))
    TEST_FAILED("Default timestep should be 60 minutes, got " + str(opt.endTime));

  // Duration strings
  if (*parse({{"timestep", "1d"}}).timeStep != 1440)
    TEST_FAILED("1d should be 1440 minutes");
  if (*parse({{"timestep", "3h"}}).timeStep != 180)
    TEST_FAILED("3h should be 180 minutes");

  // Special values
  if (parse({{"timestep", "data"}}).mode != TimeSeriesGeneratorOptions::DataTimes)
    TEST_FAILED("timestep=data should select DataTimes");
  if (parse({{"timestep", "all"}}).mode != TimeSeriesGeneratorOptions::DataTimes)
    TEST_FAILED("timestep=all should select DataTimes");
  if (parse({{"timestep", "graph"}}).mode != TimeSeriesGeneratorOptions::GraphTimes)
    TEST_FAILED("timestep=graph should select GraphTimes");

  // Invalid timesteps
  if (!fails({{"timestep", "7"}}))
    TEST_FAILED("Timestep 7 is not a divisor of 24 hours");
  if (!fails({{"timestep", "-60"}}))
    TEST_FAILED("Negative timesteps should be rejected");
  if (!fails({{"timesteps", "-1"}}))
    TEST_FAILED("Negative timesteps count should be rejected");
  if (!fails({{"timesteps", "5"}, {"endtime", "200808061200"}}))
    TEST_FAILED("timesteps and endtime are mutually exclusive");

  TEST_PASSED();
}

// ----------------------------------------------------------------------

void fixed_times()
{
  auto opt = parse({{"hour", "6,18"}});
  if (opt.mode != TimeSeriesGeneratorOptions::FixedTimes)
    TEST_FAILED("hour should select FixedTimes");
  if (opt.timeList != std::set<unsigned int>{600, 1800})
    TEST_FAILED("hour=6,18 should give times 0600 and 1800");

  opt = parse({{"time", "0630,1745"}});
  if (opt.timeList != std::set<unsigned int>{630, 1745})
    TEST_FAILED("time=0630,1745 should give times 0630 and 1745");

  if (!fails({{"hour", "24"}}))
    TEST_FAILED("hour=24 should be rejected");
  if (!fails({{"time", "2400"}}))
    TEST_FAILED("time=2400 should be rejected");
  if (!fails({{"time", "1275"}}))
    TEST_FAILED("time=1275 has invalid minutes and should be rejected");

  // A timestep cannot be combined with fixed times
  if (!fails({{"hour", "12"}, {"timestep", "60"}}))
    TEST_FAILED("hour and timestep are mutually exclusive");

  opt = parse({{"day", "1,15"}});
  if (opt.days != std::set<unsigned int>{1, 15})
    TEST_FAILED("day=1,15 should select days 1 and 15");
  if (!fails({{"day", "32"}}))
    TEST_FAILED("day=32 should be rejected");

  TEST_PASSED();
}

// ----------------------------------------------------------------------

void start_and_end_times()
{
  auto opt = parse({{"starttime", "data"}, {"endtime", "data"}});
  if (!opt.startTimeData || !opt.endTimeData)
    TEST_FAILED("starttime=data and endtime=data should take the times from the data");

  opt = parse({{"starttime", "2008-08-05T12:00:00Z"}});
  if (!opt.startTimeUTC || opt.startTime != t(5, 12))
    TEST_FAILED("A Z suffix should give a UTC start time");

  opt = parse({{"starttime", "200808051200"}, {"endtime", "200808051800"}});
  if (opt.endTime != t(5, 18) || opt.endTimeUTC)
    TEST_FAILED("End time should be 18:00 local time, got " + str(opt.endTime));

  // startstep moves the start time by whole timesteps
  opt = parse({{"starttime", "200808051200"}, {"timestep", "30"}, {"startstep", "3"}});
  if (opt.startTime != t(5, 13, 30))
    TEST_FAILED("Start time should be 3 steps of 30 minutes later, got " + str(opt.startTime));
  if (!fails({{"startstep", "-1"}}))
    TEST_FAILED("Negative startstep should be rejected");

  TEST_PASSED();
}

// ----------------------------------------------------------------------

void hash_values()
{
  auto h1 = parse({{"timestep", "30"}}).hash_value();
  auto h2 = parse({{"timestep", "60"}}).hash_value();
  auto h3 = parse({{"timestep", "30"}}).hash_value();
  if (h1 == h2)
    TEST_FAILED("Different timesteps should give different hash values");
  if (h1 != h3)
    TEST_FAILED("Equal options should give equal hash values");
  TEST_PASSED();
}

// ----------------------------------------------------------------------

class tests : public tframe::tests
{
  const char* error_message_prefix() const override { return "\n\t"; }
  void test() override
  {
    TEST(defaults);
    TEST(timesteps);
    TEST(fixed_times);
    TEST(start_and_end_times);
    TEST(hash_values);
  }
};

}  // namespace TimeSeriesGeneratorOptionsTest

int main()
{
  cout << endl
       << "TimeSeriesGeneratorOptions tester" << endl
       << "=================================" << endl;
  TimeSeriesGeneratorOptionsTest::tests t;
  return t.run();
}
