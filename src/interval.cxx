#include "forek/interval.h"
#include <stdexcept>

#include <fmt/format.h>

namespace forek::interval {

Endpoint::Endpoint(Type type, double value) : m_type{type} , m_value{value} {}

auto Endpoint::open(double value) -> Endpoint {
    return { Type::Open, value };
}

auto Endpoint::closed(double value) -> Endpoint {
    return { Type::Closed, value };
}

auto Endpoint::operator==(const Endpoint &rhs) const -> bool {
    return m_type == rhs.m_type && m_value == rhs.m_value;
}

auto Endpoint::value() const -> double {
    return m_value;
}

auto Endpoint::type() const -> Type {
    return m_type;
}

Interval::Interval(Endpoint lower, Endpoint upper)
    : m_lower{std::move(lower)}, m_upper{std::move(upper)} {
    if (lower.value() >= upper.value()) {
        throw ZeroLengthInterval{*this};
    }
}

auto Interval::lower() const noexcept -> Endpoint {
    return m_lower;
}

auto Interval::upper() const noexcept -> Endpoint {
    return m_upper;
}

Interval::operator std::string() const {
    return fmt::format("{}", *this);
}

auto Interval::operator==(const Interval &lhs) const -> bool {
    return m_lower == lhs.m_lower && m_upper == lhs.m_upper;
}

ZeroLengthInterval::ZeroLengthInterval(const Interval &i)
    : m_msg{fmt::format("Interval {} has a length of zero", i)} {}

auto start_symbol(Endpoint::Type type) -> char {
    switch(type) {
    case Endpoint::Type::Open:
        return '(';

    case Endpoint::Type::Closed:
        return '[';

    default:
        throw std::runtime_error("Unknown endpoint type");
    }
}

auto end_symbol(Endpoint::Type type) -> char {
    switch(type) {
    case Endpoint::Type::Open:
        return ')';

    case Endpoint::Type::Closed:
        return ']';

    default:
        throw std::runtime_error("Unknown endpoint type");
    }
}

auto format_as(const Interval &i) -> std::string {
    auto left_bracket = start_symbol(i.lower().type()); 
    auto start = i.lower().value();
    auto end = i.upper().value();
    auto right_bracket = end_symbol(i.upper().type());

    return fmt::format("{}{},{}{}", left_bracket, start, end, right_bracket);
}

auto operator<<(std::ostream &os, const Interval& i) -> std::ostream& {
    os << format_as(i);
    return os;
}

}  // namespace forek::interval

