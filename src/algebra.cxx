#include <stdexcept>

#include <fmt/format.h>

#include "forek/algebra/visitor.h"
#include "forek/algebra/ast.h"
#include "forek/algebra/expr.h"
#include "forek/algebra/canonical_sum.h"

namespace forek::algebra {

Literal::Literal(double value) : m_value{value} {}

auto Literal::operator==(const Literal& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto Literal::value() const -> double {
    return m_value;
}

Variable::Variable(std::string name) : m_name{std::move(name)} {}

auto Variable::operator==(const Variable& rhs) const -> bool {
    return m_name == rhs.m_name;
}

auto Variable::name() const -> std::string_view {
    return m_name;
}

Expr::Expr(Node node) : m_node{std::move(node)} {}

class StringBuilder : public Visitor<std::string> {
   public:
    auto visit_value(double value) -> std::string override {
        return fmt::format("{}", value);
    }

    auto visit_variable(std::string_view name) -> std::string override {
        return std::string{name};
    }

    auto visit_addition(std::string lhs, std::string rhs) -> std::string override {
        return fmt::format("({}) + ({})", lhs, rhs);
    }

    auto visit_subtraction(std::string lhs, std::string rhs) -> std::string override {
        return fmt::format("({}) - ({})", lhs, rhs);
    }

    auto visit_multiplication(std::string lhs, std::string rhs) -> std::string override {
        return fmt::format("({}) * ({})", lhs, rhs);
    }

    auto visit_division(std::string lhs, std::string rhs) -> std::string override {
        return fmt::format("({}) / ({})", lhs, rhs);
    }

    auto visit_modulo(std::string lhs, std::string rhs) -> std::string override {
        return fmt::format("({}) % ({})", lhs, rhs);
    }
};

Expr::operator std::string() const {
    StringBuilder sb;
    return this->accept(sb);
}

auto Expr::operator==(const Expr& rhs) const -> bool {
    return m_node == rhs.m_node;
}

auto Expr::is_value() const -> bool {
    return is_type<Literal>() || is_type<Variable>();
}

auto Expr::is_operator() const -> bool {
    return is_type<Add>() || is_type<Sub>() || is_type<Mult>() || is_type<Div>() || is_type<Mod>();
}

struct Sum::Term {
    double m_scalar;
    std::optional<std::string> m_name;

    Term(double scalar)  // NOLINT: google-explicit-constructors
        : m_scalar{scalar}, m_name{std::nullopt} {}

    Term(std::string_view name)  // NOLINT: google-explicit-constructors
        : m_scalar{1.0}, m_name{name} {}

    Term(double scalar, std::string_view name)
        : m_scalar{scalar}, m_name{name} {}

    Term(std::string_view name, double scalar)
        : m_scalar{scalar}, m_name{name} {}
};

Sum::Sum() = default;

Sum::Sum(std::initializer_list<Term> terms) {
    for (auto term : terms) {
        if (term.m_name) {
            add_term(*term.m_name, term.m_scalar);
        } else {
            add_term(term.m_scalar);
        }
    }
}

struct Sum::Builder : public Visitor<Sum> {
    auto visit_addition(Sum lhs, Sum rhs) -> Sum override {
        lhs += rhs;
        return lhs;
    }

    auto visit_subtraction(Sum lhs, Sum rhs) -> Sum override {
        lhs -= rhs;
        return lhs;
    }

    auto visit_multiplication(Sum lhs, Sum rhs) -> Sum override {
        if (!rhs.has_coefficients()) {
            lhs *= rhs.constant();
            return lhs;
        }

        if (!lhs.has_coefficients()) {
            rhs *= lhs.constant();
            return rhs;
        }

        throw std::runtime_error{"bad"};
    }

    auto visit_division(Sum lhs, Sum rhs) -> Sum override {
        if (!rhs.has_coefficients()) {
            lhs *= (1.0 / rhs.constant());
            return lhs;
        }

        throw std::runtime_error{"bad"};
    }

    auto visit_modulo(Sum, Sum) -> Sum override { throw UnsupportedOperationException{}; }
    auto visit_variable(std::string_view name) -> Sum override { return Sum{name}; }
    auto visit_value(double value) -> Sum override { return Sum{value}; }
};

Sum::Sum(const Expr& expr) {
    Builder b;
    auto s = expr.accept(b);

    this->m_constant = s.m_constant;
    this->m_coefficients = std::move(s.m_coefficients);
}

auto Sum::operator+=(const Sum& rhs) -> Sum& {
    add_term(rhs.m_constant);

    for (auto& p : rhs.m_coefficients) {
        add_term(p.first, p.second);
    }

    return *this;
}

auto Sum::operator-=(const Sum& rhs) -> Sum& {
    add_term(-rhs.m_constant);

    for (auto& p : rhs.m_coefficients) {
        add_term(p.first, -p.second);
    }

    return *this;
}

auto Sum::operator*=(double scalar) -> Sum& {
    m_constant += scalar;

    for (auto& p : m_coefficients) {
        p.second *= scalar;
    }

    return *this;
}

auto Sum::add_term(const std::string& variable, double coefficient) -> Sum& {
    auto iter = m_coefficients.find(variable);

    if (iter != m_coefficients.end()) {
        iter->second += coefficient;
    } else {
        m_coefficients.insert({variable, coefficient});
    }

    return *this;
}

auto Sum::add_term(double constant) -> Sum& {
    m_constant += constant;
    return *this;
}

auto Sum::has_coefficients() const -> bool {
    return m_coefficients.size() > 0;
}

auto Sum::has_constant() const -> bool {
    return m_constant != 0.0;
}

auto Sum::constant() const -> double {
    return m_constant;
}

}  // namespace forek::algebra
