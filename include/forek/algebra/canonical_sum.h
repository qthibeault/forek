#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>

#include "forek/algebra/expr.h"

namespace forek::algebra {

class Sum {
   private:
    struct Term;
    struct Builder;

   public:
    Sum();
    Sum(std::initializer_list<Term>);

    explicit Sum(const Expr& expr);
    explicit Sum(Term term);

    auto operator+=(const Sum& lhs) -> Sum&;
    auto operator-=(const Sum& lhs) -> Sum&;
    auto operator*=(double scalar) -> Sum&;

    auto add_term(const std::string& variable, double coefficient) -> Sum&;
    auto add_term(double constant) -> Sum&;

    [[nodiscard]] auto has_coefficients() const -> bool;
    [[nodiscard]] auto has_constant() const -> bool;
    [[nodiscard]] auto coefficients() const -> std::vector<std::pair<std::string_view, double>>;
    [[nodiscard]] auto constant() const -> double;

   private:
    double m_constant{0.0};
    std::unordered_map<std::string, double> m_coefficients{};
};

class UnsupportedOperationException : std::exception {};

auto canonical_sum(const Expr& expr) -> Sum;

}  // namespace forek::algebra
