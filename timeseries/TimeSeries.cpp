#include "TimeSeries.h"
#include <boost/make_shared.hpp>
#include <macgyver/Hash.h>
#include <algorithm>

namespace SmartMet
{
namespace TimeSeries
{
void TimeSeries::emplace_back(const TimedValue& tv)
{
  TimedValueVector::emplace_back(tv);
}

void TimeSeries::push_back(const TimedValue& tv)
{
  TimedValueVector::push_back(tv);
}

TimedValueVector::iterator TimeSeries::insert(TimedValueVector::iterator pos, const TimedValue& tv)
{
  return TimedValueVector::insert(pos, tv);
}

void TimeSeries::insert(TimedValueVector::iterator pos,
                        TimedValueVector::iterator first,
                        TimedValueVector::iterator last)
{
  TimedValueVector::insert(pos, first, last);
}

void TimeSeries::insert(TimedValueVector::iterator pos,
                        TimedValueVector::const_iterator first,
                        TimedValueVector::const_iterator last)
{
  TimedValueVector::insert(pos, first, last);
}

TimeSeries& TimeSeries::operator=(const TimeSeries& ts)
{
  if (this != &ts)
  {
    clear();
    TimedValueVector::insert(end(), ts.begin(), ts.end());
  }

  return *this;
}

LocalTimeList TimeSeries::getTimes() const
{
  LocalTimeList ret;
  std::transform(
      begin(), end(), std::back_inserter(ret), [](const auto& item) { return item.time; });
  return ret;
}

}  // namespace TimeSeries
}  // namespace SmartMet
