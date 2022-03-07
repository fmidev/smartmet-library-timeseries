#include "TimeSeriesUtility.h"
#include "TimeSeriesOutput.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


namespace SmartMet
{
namespace TimeSeries
{
// ----------------------------------------------------------------------
/*!
 * \brief
 */
// ----------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const TimeSeriesData& tsdata)
{
  try
  {
    if (boost::get<TimeSeriesPtr>(&tsdata) != nullptr)
      os << **(boost::get<TimeSeriesPtr>(&tsdata));
    else if (boost::get<TimeSeriesVectorPtr>(&tsdata) != nullptr)
      os << **(boost::get<TimeSeriesVectorPtr>(&tsdata));
    else if (boost::get<TimeSeriesGroupPtr>(&tsdata) != nullptr)
      os << **(boost::get<TimeSeriesGroupPtr>(&tsdata));

    return os;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief
 */
// ----------------------------------------------------------------------

void erase_redundant_timesteps(TimeSeries& ts,
                               const TimeSeriesGenerator::LocalTimeList& timesteps)
{
  try
  {
    // Fast special case exit
    if (ts.empty())
      return;

    // First check if all the timesteps are relevant. We assume both TimeSeries and LocalTimeList
    // are in sorted order.

    const auto n = ts.size();
    std::size_t valid_count = 0;
    std::vector<bool> keep_timestep(n, true);

    auto next_valid_time = timesteps.cbegin();
    const auto& last_valid_time = timesteps.cend();
    for (std::size_t i = 0; i < n; i++)
    {
      const auto& data = ts[i];

      // Skip valid times until data_time is greater than or equal to it
      while (next_valid_time != last_valid_time && data.time > *next_valid_time)
        ++next_valid_time;

      // Now the time is either valid (==) or not needed
      if (next_valid_time == last_valid_time || *next_valid_time != data.time)
        keep_timestep[i] = false;
      else
      {
        ++valid_count;
        ++next_valid_time;
      }
    }

    // Quick exit if nothing needs to be erased
    if (valid_count == n)
      return;

    // Create reduced timeseries and swap it with the input
    TimeSeries output(ts.getLocalTimePool());
    output.reserve(valid_count);

    for (std::size_t i = 0; i < n; i++)
    {
      if (keep_timestep[i])
        output.push_back(ts[i]);
    }

    std::swap(ts, output);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief
 */
// ----------------------------------------------------------------------

TimeSeriesPtr erase_redundant_timesteps(
    TimeSeriesPtr ts, const TimeSeriesGenerator::LocalTimeList& timesteps)
{
  try
  {
    erase_redundant_timesteps(*ts, timesteps);
    return ts;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief
 */
// ----------------------------------------------------------------------

TimeSeriesVectorPtr erase_redundant_timesteps(
    TimeSeriesVectorPtr tsv,
    const TimeSeriesGenerator::LocalTimeList& timesteps)
{
  try
  {
    for (auto& tv : *tsv)
      erase_redundant_timesteps(tv, timesteps);

    return tsv;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// ----------------------------------------------------------------------
/*!
 * \brief
 */
// ----------------------------------------------------------------------

TimeSeriesGroupPtr erase_redundant_timesteps(
    TimeSeriesGroupPtr tsg,
    const TimeSeriesGenerator::LocalTimeList& timesteps)
{
  try
  {
    for (size_t i = 0; i < tsg->size(); i++)
      erase_redundant_timesteps(tsg->at(i).timeseries, timesteps);
    return tsg;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}


}  // namespace TimeSeries
}  // namespace SmartMet
