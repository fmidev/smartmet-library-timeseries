#include "TimeSeries.h"
#include <macgyver/Exception.h>
#include <newbase/NFmiGlobals.h>
#include <spine/TableVisitor.h>

namespace SmartMet
{
using Spine::LonLat;

namespace TimeSeries
{

Spine::TableVisitor& operator<<(Spine::TableVisitor& tf, const Value& val)
{
  try
  {
    std::visit(tf, val);
    return tf;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // namespace TimeSeries
}  // namespace SmartMet
