#pragma once

#include <stdexcept>
#include <string>
#include <variant>

namespace forek::interval {
struct Inclusive {
    double value;

    explicit Inclusive(double v) : value{v} {}

    [[nodiscard]] auto open_symbol() const -> char { return '['; }
    [[nodiscard]] auto close_symbol() const -> char { return ']'; }
};

struct Exclusive {
    double value;

    explicit Exclusive(double v) : value{v} {}

    [[nodiscard]] auto open_symbol() const -> char { return '('; }
    [[nodiscard]] auto close_symbol() const -> char { return ')'; }
};

class Endpoint {
   private:
    std::variant<Inclusive, Exclusive> m_inner;

   public:
    Endpoint(Inclusive inner) : m_inner{inner} {}  // NOLINT(google-explicit-constructor)
    Endpoint(Exclusive inner) : m_inner{inner} {}  // NOLINT(google-explicit-constructor)

    template <typename T>
    struct Visitor {
        virtual auto operator()(const Exclusive &) const -> T = 0;
        virtual auto operator()(const Inclusive &) const -> T = 0;
    };

    [[nodiscard]] auto value() const -> double {
        return std::visit([](auto x) { return x.value; }, m_inner);
    }

    template <typename T>
    [[nodiscard]] auto visit(Visitor<T> &v) const -> T {
        return std::visit([&v](auto inner) { return v(inner); }, m_inner);
    }
};

struct Interval {
    Endpoint lower;
    Endpoint upper;

    Interval(Endpoint start, Endpoint end);
};

class ZeroLengthInterval : std::exception {
   private:
    std::string m_msg;

   public:
    explicit ZeroLengthInterval(const Interval &interval);

    [[nodiscard]] auto what() const noexcept -> const char * override { return m_msg.data(); }
};
}  // namespace forek::interval
