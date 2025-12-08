// ======================================================================
/*!
 * \brief Declaration of Stat class
 *
 * Stat class implements statistical functions.
 * Data can be passed in a constructor call or in addData, SetData functions.
 * Both plain double values and double-timestamp pairs are accepted as input.
 * Timestap can be either Fmi::LocalDateTime or Fmi::DateTime.
 * If valid value-timestamp pairs are passed weighted version of statistical function is called
 * unless explicitly denied by calling useWeights(false) function before.
 * Exception is sum and integ-functions. In integ-function weights are always used.
 * In sum-function weight for all values is 1.0.
 * If passed values contain missing value, statistics can not be calculated and value of
 * itsMissingValue
 * data member is returned. Missing value can be passed in constructor call or in
 * setMissingValue(double) function.
 * If useDegrees(true) is called values are handled as degrees, in that case result value is always
 * between 0...360.
 *
 */
// ======================================================================

#pragma once

#include <macgyver/DateTime.h>
#include <macgyver/LocalDateTime.h>
#include <limits>

namespace SmartMet
{
namespace TimeSeries
{
namespace Stat
{
struct DataItem
{
  DataItem(Fmi::DateTime t, double v, double w = 1.0) : time(std::move(t)), value(v), weight(w) {}

  Fmi::DateTime time = Fmi::DateTime::NOT_A_DATE_TIME;
  double value = std::numeric_limits<double>::quiet_NaN();
  double weight = 1.0;
};

std::ostream& operator<<(std::ostream& os, const DataItem& item);

using DataVector = std::vector<DataItem>;
using TimeValue = std::pair<Fmi::DateTime, double>;
using TimeValueVector = std::vector<TimeValue>;
using LocalTimeValue = std::pair<Fmi::LocalDateTime, double>;
using LocalTimeValueVector = std::vector<LocalTimeValue>;

class Stat
{
 public:
  Stat(double theMissingValue = std::numeric_limits<double>::quiet_NaN());
  Stat(const std::vector<double>& theValues,
       double theMissingValue = std::numeric_limits<double>::quiet_NaN());
  Stat(const LocalTimeValueVector& theValues,
       double theMissingValue = std::numeric_limits<double>::quiet_NaN());
  Stat(const TimeValueVector& theValues,
       double theMissingValue = std::numeric_limits<double>::quiet_NaN());
  Stat(const DataVector& theValues,
       double theMissingValue = std::numeric_limits<double>::quiet_NaN());

  void setData(const DataVector& theValues);
  void addData(double theValue);
  void addData(const Fmi::LocalDateTime& theTime, double theValue);
  void addData(const Fmi::DateTime& theTime, double theValue);
  void addData(const std::vector<double>& theValues);
  void addData(const DataItem& theValue);
  void setMissingValue(double theMissingValue) { itsMissingValue = theMissingValue; }
  void useWeights(bool theWeights = true) { itsWeights = theWeights; }
  void useDegrees(bool theDegrees = true) { itsDegrees = theDegrees; }
  void clear();

  double integ(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
               const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double sum(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
             const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double min(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
             const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double mean(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
              const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double max(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
             const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double change(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double trend(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
               const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  unsigned int count(double lowerLimit,
                     double upperLimit,
                     const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                     const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double percentage(double lowerLimit,
                    double upperLimit,
                    const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                    const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double median(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double variance(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                  const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double stddev(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double nearest(const Fmi::DateTime& timestep,
                 const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                 const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double interpolate(const Fmi::DateTime& timestep,
                     const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                     const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  double circlemean(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                    const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;

 private:
  double stddev_dir(const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                    const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME) const;
  bool get_subvector(DataVector& subvector,
                     const Fmi::DateTime& startTime = Fmi::DateTime::NOT_A_DATE_TIME,
                     const Fmi::DateTime& endTime = Fmi::DateTime::NOT_A_DATE_TIME,
                     bool useWeights = true) const;
  void calculate_weights();
  bool invalid_timestamps() const;

  DataVector itsData;
  double itsMissingValue = 0;
  bool itsWeights = false;
  bool itsDegrees = false;
};

}  // namespace Stat
}  // namespace TimeSeries
}  // namespace SmartMet

// ======================================================================
