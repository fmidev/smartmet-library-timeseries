#pragma once

#include "TimeSeries.h"
#include <boost/date_time/local_time/local_time.hpp>
#include <macgyver/ValueFormatter.h>

namespace SmartMet {
namespace TimeSeries {
// write content of Value to ostream (not formatted)
std::ostream &operator<<(std::ostream &os, const Value &val);
// write content of TimeSeries to ostream (not formatted)
std::ostream &operator<<(std::ostream &os, const TimeSeries &ts);
// write content of TimeSeriesGroup to ostream (not formatted)
std::ostream &operator<<(std::ostream &os, const TimeSeriesGroup &tsg);
// write content of TimeSeriesVector to ostream (not formatted)
std::ostream &operator<<(std::ostream &os, const TimeSeriesVector &tsv);

enum class LonLatFormat { LATLON, LONLAT };

// format Value and write to output stream
// usage: boost::apply_visitor(ostream_visitor, Value);
class OStreamVisitor : public boost::static_visitor<> {
private:
  std::ostream &itsOutstream;
  const Fmi::ValueFormatter &itsValueFormatter;
  int itsPrecision;
  LonLatFormat itsLonLatFormat;

public:
  OStreamVisitor(std::ostream &outs, const Fmi::ValueFormatter &valueformatter,
                 int precision)
      : itsOutstream(outs), itsValueFormatter(valueformatter),
        itsPrecision(precision), itsLonLatFormat(LonLatFormat::LONLAT) {}

  void operator()(const None &none) const;
  void operator()(const std::string &str) const;
  void operator()(double d) const;
  void operator()(int i) const;
  void operator()(const LonLat &lonlat) const;
  void operator()(const boost::local_time::local_date_time &ldt) const;

  // Set LonLat - value formatting
  OStreamVisitor &operator<<(LonLatFormat newformat);
};

// format Value into string
// usage: boost::apply_visitor(ostream_visitor, Value);
class StringVisitor : public boost::static_visitor<std::string> {
private:
  const Fmi::ValueFormatter &itsValueFormatter;
  int itsPrecision;
  LonLatFormat itsLonLatFormat;

public:
  StringVisitor(const Fmi::ValueFormatter &valueformatter, int precision)
      : itsValueFormatter(valueformatter), itsPrecision(precision),
        itsLonLatFormat(LonLatFormat::LONLAT) {}

  void setLonLatFormat(LonLatFormat newFormat) { itsLonLatFormat = newFormat; }
  void setPrecision(int newPrecision) { itsPrecision = newPrecision; }
  std::string operator()(const None &none) const;
  std::string operator()(const std::string &str) const;
  std::string operator()(double d) const;
  std::string operator()(int i) const;
  std::string operator()(const LonLat &lonlat) const;
  std::string operator()(const boost::local_time::local_date_time &ldt) const;
};

} // namespace TimeSeries
} // namespace SmartMet
