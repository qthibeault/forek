#include "forek/interval.h"

#include <fmt/core.h>
#include <fmt/format.h>

using forek::interval::Endpoint;
using forek::interval::Exclusive;
using forek::interval::Inclusive;
using forek::interval::Interval;
using forek::interval::ZeroLengthInterval;

struct OpenSymbolVisitor : public Endpoint::Visitor<char> {
    auto operator()(const Exclusive &) const -> char override { return '('; }
    auto operator()(const Inclusive &) const -> char override { return '['; }
};

struct CloseSymbolVisitor : public Endpoint::Visitor<char> {
    auto operator()(const Exclusive &) const -> char override { return ')'; }
    auto operator()(const Inclusive &) const -> char override { return ']'; }
};

Interval::Interval(Endpoint lower, Endpoint upper)
    : m_lower{std::move(lower)}, m_upper{std::move(upper)} {
    if (lower.value() >= upper.value()) {
        throw ZeroLengthInterval{*this};
    }
}

template <>
struct fmt::formatter<Interval> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Interval &i, FormatContext &ctx) {
        auto open_visitor = OpenSymbolVisitor{};
        auto open = i.lower().visit(open_visitor);
        auto start = i.lower().value();

        auto close_visitor = CloseSymbolVisitor{};
        auto close = i.upper().visit(close_visitor);
        auto end = i.upper().value();

        return fmt::format_to(ctx.out(), "{}{},{}{}", open, start, end, close);
    }
};

Interval::operator std::string() const { return fmt::format("{}", *this); }

ZeroLengthInterval::ZeroLengthInterval(const Interval &i)
    : m_msg{fmt::format("Interval {} has a length of zero", i)} {}

auto forek::interval::make_inclusive(double value) -> Endpoint { return {Inclusive{value}}; }
auto forek::interval::make_exclusive(double value) -> Endpoint { return {Exclusive{value}}; }
