#pragma once

#include <optional>
#include <span>
#include <string>
#include <vector>

namespace forek::algebra {

class Term {
    double m_scalar;
    std::optional<std::string> m_name;

   public:
    Term(std::string name, double scalar) : m_scalar{scalar}, m_name{std::move(name)} {}
    Term(double scalar, std::string name) : m_scalar{scalar}, m_name{std::move(name)} {}
    explicit Term(double scalar) : m_scalar{scalar}, m_name{std::nullopt} {}

    auto scalar() -> double { return m_scalar; }
    auto name() -> std::optional<std::string>& { return m_name; }
};

class Sum {
    std::vector<Term> m_terms;

   public:
    explicit Sum(std::vector<Term> terms) : m_terms{std::move(terms)} {}

    [[nodiscard]] auto terms() const -> std::span<const Term> { return {m_terms}; }
};

enum class Comparison { GreaterThan, GreaterThenEquals, LessThan, LessThanEquals };

}  // namespace forek::algebra
