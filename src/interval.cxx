#include "forek/interval.h"

#include <fmt/core.h>
#include <fmt/format.h>

namespace forek::interval {

Inclusive::Inclusive(double value) : m_value{value} {}

auto Inclusive::operator==(const Inclusive &lhs) const -> bool {
    return m_value == lhs.m_value;
}

auto Inclusive::value() const noexcept -> double {
    return m_value;
}

Exclusive::Exclusive(double value) : m_value{value} {}

auto Exclusive::operator==(const Exclusive &lhs) const -> bool {
    return m_value == lhs.m_value;
}

auto Exclusive::value() const noexcept -> double {
    return m_value;
}

Endpoint::Endpoint(Inclusive inner) : m_inner{inner} {}  // NOLINT(google-explicit-constructor)
Endpoint::Endpoint(Exclusive inner) : m_inner{inner} {}  // NOLINT(google-explicit-constructor)

auto Endpoint::operator==(const Endpoint &lhs) const -> bool {
    return m_inner == lhs.m_inner;
}

auto Endpoint::value() const -> double {
    return std::visit([](auto x) { return x.value(); }, m_inner);
}

template <typename T>
auto Endpoint::visit(Visitor<T> &v) const -> T {
    return std::visit([&v](auto inner) { return v(inner); }, m_inner);
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

auto make_inclusive(double value) -> Endpoint {
    return {Inclusive{value}};
}

auto make_exclusive(double value) -> Endpoint {
    return {Exclusive{value}};
}

}  // namespace forek::interval

namespace fmt {

using forek::interval::Interval;

auto formatter<Interval>::format(const Interval &i, format_context &ctx)
    -> format_context::iterator {
    using forek::interval::Endpoint;
    using forek::interval::Exclusive;
    using forek::interval::Inclusive;

    struct OpenSymbolVisitor : public Endpoint::Visitor<char> {
        auto operator()(const Exclusive &) const -> char override { return '('; }
        auto operator()(const Inclusive &) const -> char override { return '['; }
    };

    struct CloseSymbolVisitor : public Endpoint::Visitor<char> {
        auto operator()(const Exclusive &) const -> char override { return ')'; }
        auto operator()(const Inclusive &) const -> char override { return ']'; }
    };

    auto open_visitor = OpenSymbolVisitor{};
    auto open = i.lower().visit(open_visitor);
    auto start = i.lower().value();

    auto close_visitor = CloseSymbolVisitor{};
    auto close = i.upper().visit(close_visitor);
    auto end = i.upper().value();

    return fmt::format_to(ctx.out(), "{}{},{}{}", open, start, end, close);
}

}  // namespace fmt
