#include "TimeSeriesOutput.h"
#include <macgyver/Exception.h>
#include <macgyver/StringConversion.h>
#include <spine/LonLat.h>
#include <spine/None.h>

using ::SmartMet::Spine::LonLat;
using ::SmartMet::Spine::LonLatFormat;
using ::SmartMet::Spine::None;

namespace SmartMet
{
namespace TimeSeries
{
std::ostream& operator<<(std::ostream& os, const Value& val)
{
  try
  {
    if (const auto* ptr = std::get_if<double>(&val))
      os << *ptr;
    else if (const auto* ptr = std::get_if<int>(&val))
      os << *ptr;
    else if (const auto* ptr = std::get_if<std::string>(&val))
      os << *ptr;
    else if (const auto* ptr = std::get_if<LonLat>(&val))
    {
      LonLat coord = *ptr;
      os << coord.lon << ", " << coord.lat;
    }
    else if (const auto* ptr = std::get_if<Fmi::LocalDateTime>(&val))
    {
      Fmi::LocalDateTime ldt = *ptr;
      os << ldt;
    }
    else if (std::get_if<None>(&val))
    {
      os << "nan";
    }

    return os;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::ostream& operator<<(std::ostream& os, const TimeSeries& ts)
{
  try
  {
    for (const auto& t : ts)
      os << t.time << " -> " << t.value << std::endl;

    return os;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::ostream& operator<<(std::ostream& os, const TimeSeriesGroup& tsg)
{
  try
  {
    for (unsigned int i = 0; i < tsg.size(); i++)
    {
      if (i > 0)
        os << std::endl;
      //		  os << "location #" << i << " (lon, lat): " << tsg[i].lonlat.lon << ", " <<
      // tsg[i].lonlat.lat  << std::endl;
      os << tsg[i].timeseries;
    }

    return os;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::ostream& operator<<(std::ostream& os, const TimeSeriesVector& tsv)
{
  try
  {
    for (unsigned int i = 0; i < tsv.size(); i++)
    {
      os << "column #" << i << std::endl;
      os << tsv[i];
    }

    return os;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

// StringVisitor

std::string StringVisitor::operator()(const None& /* none */) const
{
  try
  {
    return itsValueFormatter.missing();
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::string StringVisitor::operator()(const std::string& str) const
{
  return str;
}

std::string StringVisitor::operator()(double d) const
{
  try
  {
    return itsValueFormatter.format(d, itsPrecision);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::string StringVisitor::operator()(int i) const
{
  try
  {
    return Fmi::to_string(i);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::string StringVisitor::operator()(const LonLat& lonlat) const
{
  try
  {
    if (itsLonLatFormat == LonLatFormat::LONLAT)
      return itsValueFormatter.format(lonlat.lon, itsPrecision) + ", " +
             itsValueFormatter.format(lonlat.lat, itsPrecision);

    return itsValueFormatter.format(lonlat.lat, itsPrecision) + ", " +
           itsValueFormatter.format(lonlat.lon, itsPrecision);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

std::string StringVisitor::operator()(const Fmi::LocalDateTime& ldt) const
{
  try
  {
    return Fmi::to_iso_extended_string(ldt.local_time());
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

/* OStreamVisitor */
void OStreamVisitor::operator()(const None& /* none */) const
{
  try
  {
    itsOutstream << itsValueFormatter.missing();
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

void OStreamVisitor::operator()(const std::string& str) const
{
  try
  {
    itsOutstream << str;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

void OStreamVisitor::operator()(double d) const
{
  try
  {
    itsOutstream << itsValueFormatter.format(d, itsPrecision);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

void OStreamVisitor::operator()(int i) const
{
  try
  {
    itsOutstream << i;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

void OStreamVisitor::operator()(const LonLat& lonlat) const
{
  try
  {
    itsOutstream << itsValueFormatter.format(lonlat.lon, itsPrecision) << ", "
                 << itsValueFormatter.format(lonlat.lat, itsPrecision);
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

void OStreamVisitor::operator()(const Fmi::LocalDateTime& ldt) const
{
  try
  {
    itsOutstream << ldt;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

OStreamVisitor& OStreamVisitor::operator<<(LonLatFormat newformat)
{
  try
  {
    itsLonLatFormat = newformat;
    return *this;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // namespace TimeSeries
}  // namespace SmartMet
