#include "TimeSeries.h"

using namespace SmartMet::TimeSeries;

namespace
{
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
}

bool Value::operator == (const Value& other) const
{
    const auto ind = index();
    if (ind != other.index())
        return false;

    return std::visit(EqualVisitor(other), dynamic_cast<const Value_&>(*this));
}
