#pragma once

#include <string_view>

namespace forek::algebra {

template <typename T>
struct Visitor {
    Visitor() = default;
    Visitor(const Visitor<T>&) = default;
    Visitor(Visitor<T>&&) = default;

    auto operator=(const Visitor&) -> Visitor& = default;
    auto operator=(Visitor&&) -> Visitor& = default;

    virtual auto visit_value(double value) -> T = 0;
    virtual auto visit_variable(std::string_view name) -> T = 0;
    virtual auto visit_addition(T lhs, T rhs) -> T = 0;
    virtual auto visit_subtraction(T lhs, T rhs) -> T = 0;
    virtual auto visit_multiplication(T lhs, T rhs) -> T = 0;
    virtual auto visit_division(T lhs, T rhs) -> T = 0;
    virtual auto visit_modulo(T lhs, T rhs) -> T = 0;
    virtual ~Visitor() = default;
};

}  // namespace forek::algebra
