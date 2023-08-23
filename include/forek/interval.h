#pragma once

#include <stdexcept>
#include <string>
#include <variant>

namespace forek::interval {
class Inclusive {
    double m_value;

   public:
    explicit Inclusive(double value) : m_value{value} {}

    [[nodiscard]] auto value() const noexcept -> double { return m_value; }
};

class Exclusive {
    double m_value;

   public:
    explicit Exclusive(double value) : m_value{value} {}

    [[nodiscard]] auto value() const noexcept -> double { return m_value; }
};

class Endpoint {
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
        return std::visit([](auto x) { return x.value(); }, m_inner);
    }

    template <typename T>
    [[nodiscard]] auto visit(Visitor<T> &v) const -> T {
        return std::visit([&v](auto inner) { return v(inner); }, m_inner);
    }
};

class Interval {
    Endpoint m_lower;
    Endpoint m_upper;

   public:
    Interval(Endpoint start, Endpoint end);

    [[nodiscard]] auto lower() const noexcept -> Endpoint { return m_lower; }
    [[nodiscard]] auto upper() const noexcept -> Endpoint { return m_upper; }
};

class ZeroLengthInterval : std::exception {
   private:
    std::string m_msg;

   public:
    explicit ZeroLengthInterval(const Interval &interval);

    [[nodiscard]] auto what() const noexcept -> const char * override { return m_msg.data(); }
};
}  // namespace forek::interval
