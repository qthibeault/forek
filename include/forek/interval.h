#pragma once

#include <stdexcept>
#include <variant>

namespace forek::interval {
struct Inclusive {
    const double value;

    Inclusive() = delete;
    Inclusive(const Inclusive&) = default;
    Inclusive(Inclusive&&) = default;

    explicit Inclusive(double v) : value{v} {}

    [[nodiscard]] auto open_symbol() const -> char { return '['; }
    [[nodiscard]] auto close_symbol() const -> char { return ']'; }
};

struct Exclusive {
    const double value;

    Exclusive() = delete;
    Exclusive(const Exclusive&) = default;
    Exclusive(Exclusive&&) = default;

    explicit Exclusive(double v) : value{v} {}

    [[nodiscard]] auto open_symbol() const -> char { return '('; }
    [[nodiscard]] auto close_symbol() const -> char { return ')'; }
};

class Endpoint {
   private:
    const std::variant<Inclusive, Exclusive> m_inner;

   public:
    Endpoint() = delete;
    Endpoint(Inclusive inner) : m_inner{inner} {}  // NOLINT(google-explicit-constructor)
    Endpoint(Exclusive inner) : m_inner{inner} {}  // NOLINT(google-explicit-constructor)

    template<typename T>
    struct Visitor {
        virtual auto operator()(const Exclusive&) const -> T = 0;
        virtual auto operator()(const Inclusive&) const -> T = 0;
    };

    [[nodiscard]] auto value() const -> double {
        return std::visit([](auto x) { return x.value; }, m_inner);
    }

    template <typename T>
    [[nodiscard]] auto visit(Visitor<T>& v) const -> T {
        return std::visit([&v](auto inner){ return v(inner); }, m_inner);
    }
};

struct Interval {
    const Endpoint lower;
    const Endpoint upper;

    Interval() = delete;
    Interval(const Interval&) = default;
    Interval(Interval&&) = default;
    Interval(Endpoint start, Endpoint end);

    explicit operator std::string() const;
};

class ZeroLengthInterval : std::exception {
   private:
    const std::string m_msg;

   public:
    ZeroLengthInterval() = delete;
    ZeroLengthInterval(const ZeroLengthInterval&) = default;
    ZeroLengthInterval(ZeroLengthInterval&&) = default;

    explicit ZeroLengthInterval(const Interval& interval);

    [[nodiscard]] auto what() const noexcept -> const char* override { return m_msg.data(); }
};
}  // namespace forek::interval
