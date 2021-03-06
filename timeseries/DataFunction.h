// ======================================================================
/*!
 * \brief Interface of class DataFunction
 */
// ======================================================================

#pragma once

#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace SmartMet
{
namespace TimeSeries
{
// 7 days
static const int MAX_AGGREGATION_INTERVAL = (7 * 24 * 60);

enum class FunctionId
{
  Mean = 0x1,
  Maximum,
  Minimum,
  Median,
  Sum,
  Integ,
  StandardDeviation,
  Percentage,
  Count,
  Change,
  Trend,
  Nearest,
  Interpolate,
  NullFunction
};

enum class FunctionType
{
  TimeFunction = 0x0,
  AreaFunction,
  NullFunctionType
};

struct DataFunction
{
  DataFunction()
      : itsFunctionId(FunctionId::NullFunction),
        itsFunctionType(FunctionType::NullFunctionType),
        itsLowerLimit(std::numeric_limits<double>::lowest()),
        itsUpperLimit(std::numeric_limits<double>::max()),
        itsAggregationIntervalBehind(MAX_AGGREGATION_INTERVAL),
        itsAggregationIntervalAhead(MAX_AGGREGATION_INTERVAL),
        itsNaNFunction(false)
  {
    if (itsFunctionId == FunctionId::Nearest || itsFunctionId == FunctionId::Interpolate)
      itsNaNFunction = true;
  }
  DataFunction(FunctionId theFunctionId,
                    FunctionType theFunctionType,
                    double theLowerLimit = std::numeric_limits<double>::lowest(),
                    double theUpperLimit = std::numeric_limits<double>::max())
      : itsFunctionId(theFunctionId),
        itsFunctionType(theFunctionType),
        itsLowerLimit(theLowerLimit),
        itsUpperLimit(theUpperLimit),
        itsAggregationIntervalBehind(MAX_AGGREGATION_INTERVAL),
        itsAggregationIntervalAhead(MAX_AGGREGATION_INTERVAL),
        itsNaNFunction(false)
  {
    if (itsFunctionId == FunctionId::Nearest || itsFunctionId == FunctionId::Interpolate)
      itsNaNFunction = true;
  }
  DataFunction(const DataFunction& pf) = default;
  DataFunction& operator=(const DataFunction& pf) = default;
  bool exists() const { return itsFunctionType != FunctionType::NullFunctionType; }
  std::string info() const;
  FunctionId id() const { return itsFunctionId; }
  FunctionType type() const { return itsFunctionType; }
  double lowerLimit() const { return itsLowerLimit; }
  double upperLimit() const { return itsUpperLimit; }
  bool lowerOrUpperLimitGiven() const
  {
    return itsLowerLimit != std::numeric_limits<double>::lowest() ||
           itsUpperLimit != std::numeric_limits<double>::max();
  }
  unsigned int getAggregationIntervalBehind() const { return itsAggregationIntervalBehind; }
  unsigned int getAggregationIntervalAhead() const { return itsAggregationIntervalAhead; }
  void setAggregationIntervalBehind(unsigned int theIntervalBehind)
  {
    itsAggregationIntervalBehind = theIntervalBehind;
  }
  void setAggregationIntervalAhead(unsigned int theIntervalAhead)
  {
    itsAggregationIntervalAhead = theIntervalAhead;
  }
  bool isNanFunction() const { return itsNaNFunction; }
  std::string hash() const;
  void setLimits(double theLowerLimit, double theUpperLimit)
  {
    itsLowerLimit = theLowerLimit;
    itsUpperLimit = theUpperLimit;
  }
  void setId(FunctionId id) { itsFunctionId = id; }
  void setType(FunctionType type) { itsFunctionType = type; }
  void setIsNaNFunction(bool nan) { itsNaNFunction = nan; }

  friend std::ostream& operator<<(std::ostream& out, const DataFunction& func);

 private:
  FunctionId itsFunctionId;
  FunctionType itsFunctionType;
  double itsLowerLimit;
  double itsUpperLimit;
  unsigned int itsAggregationIntervalBehind;
  unsigned int itsAggregationIntervalAhead;
  bool itsNaNFunction;
};

struct DataFunctions
{
  DataFunctions() = default;
  DataFunctions(const DataFunction& theInnerFunction,
                     const DataFunction& theOuterFunction)
      : innerFunction(theInnerFunction), outerFunction(theOuterFunction)
  {
  }
  DataFunctions(const DataFunctions& functions) = default;
  DataFunctions& operator=(const DataFunctions& other) = default;

  DataFunction innerFunction;
  DataFunction outerFunction;

  friend std::ostream& operator<<(std::ostream& out, const DataFunctions& func);
};

std::ostream& operator<<(std::ostream& out, const DataFunction& func);
std::ostream& operator<<(std::ostream& out, const DataFunctions& funcs);

}  // namespace TimeSeries
}  // namespace SmartMet

// ======================================================================
