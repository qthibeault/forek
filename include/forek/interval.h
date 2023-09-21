#pragma once

#include <ostream>
#include <stdexcept>
#include <string>
#include <variant>

namespace forek::interval {

class Endpoint {
   public:
    enum class Type { Open, Closed };

    static auto open(double value) -> Endpoint;
    static auto closed(double value) -> Endpoint;
    auto operator==(const Endpoint& lhs) const -> bool;

    [[nodiscard]] auto type() const -> Type;
    [[nodiscard]] auto value() const -> double;

   private:
    Type m_type;
    double m_value;

    Endpoint(Type type, double value);
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

auto format_as(const Interval &i) -> std::string;
auto operator<<(std::ostream &os, const Interval &i) -> std::ostream&;

class ZeroLengthInterval : std::exception {
   public:
    explicit ZeroLengthInterval(const Interval &interval);

    [[nodiscard]] auto what() const noexcept -> const char * override { return m_msg.data(); }

   private:
    std::string m_msg;
};

}  // namespace forek::interval
