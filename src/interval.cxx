#include <fmt/core.h>
#include <fmt/format.h>

#include "forek/interval.h"

using forek::interval::Endpoint;
using forek::interval::Exclusive;
using forek::interval::Inclusive;
using forek::interval::Interval;
using forek::interval::ZeroLengthInterval;

struct OpenSymbolVisitor : public Endpoint::Visitor<char> {
    auto operator()(const Exclusive&) const -> char override { return '('; }
    auto operator()(const Inclusive&) const -> char override { return '('; }
};

struct CloseSymbolVisitor : public Endpoint::Visitor<char> {
    auto operator()(const Exclusive&) const -> char override { return ')'; }
    auto operator()(const Inclusive&) const -> char override { return ')'; }
};

Interval::Interval(Endpoint lower, Endpoint upper)
    : lower{std::move(lower)}, upper{std::move(upper)} {
    if (lower.value() >= upper.value()) {
        throw ZeroLengthInterval{*this};
    }
}

Interval::operator std::string() const {
    auto open_visitor = OpenSymbolVisitor{};
    auto open = lower.visit(open_visitor);
    auto start = lower.value();

    auto close_visitor = CloseSymbolVisitor{};
    auto close = upper.visit(close_visitor);
    auto end = upper.value();

    return fmt::format("{}{},{}{}", open, start, end, close);
}

ZeroLengthInterval::ZeroLengthInterval(const Interval& i)
    : m_msg{fmt::format("Interval {} has a length of zero", std::string{i})} {}
