#include "TimeSeries.h"
#include "TimeSeriesOutput.h"
#include "TimeSeriesOutput.h"
#include <macgyver/TypeName.h>
#include <macgyver/Exception.h>
#include <macgyver/StringConversion.h>
#include <macgyver/ValueFormatter.h>
#include <typeinfo>

using namespace SmartMet::TimeSeries;

namespace
{
    std::string as_string(const Value& value)
    {
        Fmi::ValueFormatterParam vfp;
        Fmi::ValueFormatter vf(vfp);
        return value.apply_visitor(StringVisitor(vf, 6));
    }

    struct EqualVisitor
    {
        EqualVisitor(const Value& other) : other(other) {}

        template <typename Type>
        bool operator () (const Type& arg)
        {
            bool is_equal = arg == std::get<Type>(other);
            return is_equal;
        }

        const Value& other;
    };

    template <typename From, typename To>
    [[noreturn]] void report_bad_conversion()
    {
        Fmi::Exception error(BCP, "Conversion from "
            + Fmi::demangle_cpp_type_name(typeid(From).name())
            + " to " + Fmi::demangle_cpp_type_name(typeid(To).name())
            + " is not supported");
        throw error;
    }

    struct GetDoubleVisitor
    {
        template <typename Type> double operator () (const Type&) const { report_bad_conversion<Type, double>(); }
    };

    template<> double GetDoubleVisitor::operator() (const int& arg) const { return arg; }
    template<> double GetDoubleVisitor::operator() (const double& arg) const { return arg; }
    template<> double GetDoubleVisitor::operator() (const std::string& arg) const { return Fmi::stod(arg); }

    struct GetIntVisitor
    {
        template <typename Type> int operator () (const Type& arg) const { report_bad_conversion<Type, int>(); }
    };

    template<> int GetIntVisitor::operator() (const int& arg) const { return arg; }
    template<> int GetIntVisitor::operator() (const double& arg) const { return arg; } // FIXME: check range
    template<> int GetIntVisitor::operator() (const std::string& arg) const { return Fmi::stoi(arg); }
}

bool Value::operator == (const Value& other) const
{
    const auto ind = index();
    if (ind != other.index())
        return false;

    EqualVisitor visitor(other);
    return this->apply_visitor(visitor);
}

int Value::as_int() const
try
{
    return apply_visitor(GetIntVisitor());
}
catch (...)
{
    auto error = Fmi::Exception::Trace(BCP, "Operation failed");
    error.addParameter("Value", as_string(*this));
    throw error;
}

double Value::as_double() const
try
{
    return apply_visitor(GetDoubleVisitor());
}
catch (...)
{
    Fmi::ValueFormatterParam vfp;
    Fmi::ValueFormatter vf(vfp);
    auto error = Fmi::Exception::Trace(BCP, "Operation failed");
    error.addParameter("Value", as_string(*this));
    throw error;
}
