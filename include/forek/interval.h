#pragma once

#include <stdexcept>
#include <string>
#include <variant>

#include "fmt/core.h"

namespace forek::interval {

class Inclusive {
   public:
    explicit Inclusive(double value);

    auto operator==(const Inclusive& lhs) const -> bool;

    [[nodiscard]] auto value() const noexcept -> double;

   private:
    double m_value;
};

class Exclusive {
   public:
    explicit Exclusive(double value);

    auto operator==(const Exclusive& lhs) const -> bool;

    [[nodiscard]] auto value() const noexcept -> double;

   private:
    double m_value;
};

class Endpoint {
   public:
    Endpoint(Inclusive inner);  // NOLINT(google-explicit-constructor)
    Endpoint(Exclusive inner);  // NOLINT(google-explicit-constructor)

    template <typename T>
    struct Visitor {
        Visitor() = default;
        Visitor(const Visitor &) = default;
        Visitor(Visitor &&) = default;

        auto operator=(const Visitor &) -> Visitor & = default;
        auto operator=(Visitor &&) -> Visitor & = default;

        virtual auto operator()(const Exclusive &) const -> T = 0;
        virtual auto operator()(const Inclusive &) const -> T = 0;
        virtual ~Visitor() = default;
    };

    auto operator==(const Endpoint& lhs) const -> bool;

    [[nodiscard]] auto value() const -> double;

    template <typename T>
    [[nodiscard]] auto visit(Visitor<T> &v) const -> T;

   private:
    std::variant<Inclusive, Exclusive> m_inner;

};

class Interval {
   public:
    Interval(Endpoint start, Endpoint end);

    explicit operator std::string() const;
    auto operator==(const Interval& lhs) const -> bool;

    [[nodiscard]] auto lower() const noexcept -> Endpoint;
    [[nodiscard]] auto upper() const noexcept -> Endpoint;

   private:
    Endpoint m_lower;
    Endpoint m_upper;
};

class ZeroLengthInterval : std::exception {
   public:
    explicit ZeroLengthInterval(const Interval &interval);

    [[nodiscard]] auto what() const noexcept -> const char * override { return m_msg.data(); }

   private:
    std::string m_msg;
};

auto make_inclusive(double value) -> Endpoint;
auto make_exclusive(double value) -> Endpoint;

}  // namespace forek::interval

namespace fmt {

template <>
struct fmt::formatter<forek::interval::Interval> {
    constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const forek::interval::Interval &i, format_context &ctx) -> format_context::iterator;
};

}  // namespace fmt
