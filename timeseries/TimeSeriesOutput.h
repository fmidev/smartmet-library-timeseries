#pragma once

#include "TimeSeries.h"
#include <macgyver/ValueFormatter.h>
#include <spine/LonLat.h>

namespace SmartMet
{
namespace TimeSeries
{
// write content of Value to ostream (not formatted)
std::ostream &operator<<(std::ostream &os, const ::SmartMet::TimeSeries::Value &val);
// write content of TimeSeries to ostream (not formatted)
std::ostream &operator<<(std::ostream &os, const TimeSeries &ts);
// write content of TimeSeriesGroup to ostream (not formatted)
std::ostream &operator<<(std::ostream &os, const TimeSeriesGroup &tsg);
// write content of TimeSeriesVector to ostream (not formatted)
std::ostream &operator<<(std::ostream &os, const TimeSeriesVector &tsv);

// format Value and write to output stream
// usage: boost::apply_visitor(ostream_visitor, Value);
class OStreamVisitor
{
 private:
  std::ostream &itsOutstream;
  const Fmi::ValueFormatter &itsValueFormatter;
  int itsPrecision;
  Spine::LonLatFormat itsLonLatFormat = Spine::LonLatFormat::LONLAT;

 public:
  OStreamVisitor(std::ostream &outs, const Fmi::ValueFormatter &valueformatter, int precision)
      : itsOutstream(outs), itsValueFormatter(valueformatter), itsPrecision(precision)
  {
  }

  void operator()(const Spine::None &none) const;
  void operator()(const std::string &str) const;
  void operator()(double d) const;
  void operator()(int i) const;
  void operator()(const Spine::LonLat &lonlat) const;
  void operator()(const Fmi::LocalDateTime &ldt) const;

  // Set LonLat - value formatting
  OStreamVisitor &operator<<(Spine::LonLatFormat newformat);
};

// format Value into string
// usage: boost::apply_visitor(ostream_visitor, Value);
class StringVisitor
{
 private:
  const Fmi::ValueFormatter &itsValueFormatter;
  int itsPrecision;
  Spine::LonLatFormat itsLonLatFormat = Spine::LonLatFormat::LONLAT;

 public:
  StringVisitor(const Fmi::ValueFormatter &valueformatter, int precision)
      : itsValueFormatter(valueformatter), itsPrecision(precision)
  {
  }

  void setLonLatFormat(Spine::LonLatFormat newFormat) { itsLonLatFormat = newFormat; }
  void setPrecision(int newPrecision) { itsPrecision = newPrecision; }
  std::string operator()(const Spine::None &none) const;
  std::string operator()(const std::string &str) const;
  std::string operator()(double d) const;
  std::string operator()(int i) const;
  std::string operator()(const Spine::LonLat &lonlat) const;
  std::string operator()(const Fmi::LocalDateTime &ldt) const;
};

}  // namespace TimeSeries
}  // namespace SmartMet
