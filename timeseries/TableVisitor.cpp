#include "TimeSeries.h"
#include <macgyver/Exception.h>
#include <newbase/NFmiGlobals.h>
#include <spine/TableVisitor.h>

namespace SmartMet
{
namespace TimeSeries
{

Spine::TableVisitor& operator<<(Spine::TableVisitor& tf, const Value& val)
{
  try
  {
    val.apply_visitor(tf);
    return tf;
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

}  // namespace TimeSeries
}  // namespace SmartMet
