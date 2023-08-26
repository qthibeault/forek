#include "forek/algebra.h"
#include <stdexcept>

using forek::algebra::Expr;
using forek::algebra::Sum;
using forek::algebra::UnsupportedOperationException;
using forek::algebra::Visitor;

Sum::Sum() : m_constant{0.0}, m_coefficients{} {}
Sum::Sum(std::initializer_list<Term> terms) : m_constant{0.0}, m_coefficients{} {
    for (auto term : terms) {
        if (term.m_name) {
            add_term(*term.m_name, term.m_scalar);
        } else {
            add_term(term.m_scalar);
        }
    }
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

struct SumVisitor : public Visitor<Sum> {
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

    auto visit_modulo(Sum lhs, Sum rhs) -> Sum override {
        throw UnsupportedOperationException{};
    }

    auto visit_variable(std::string name) -> Sum override {
        return Sum{std::move(name)};
    }

    auto visit_value(double value) -> Sum override {
        return Sum{value};
    }
};

auto forek::algebra::canonical_sum(const Expr &expr) -> Sum {
    SumVisitor visitor;
    return expr.evaluate(visitor);
}
