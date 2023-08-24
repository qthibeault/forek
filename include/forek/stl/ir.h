#pragma once

#include <optional>
#include <string>
#include <vector>

#include "forek/mtl/ir.h"

namespace forek::ir {
struct Predicate {
    class Term {
        double m_scalar;
        std::optional<std::string> m_name;

       public:
        Term(std::string name, double scalar) : m_scalar{scalar}, m_name{std::move(name)} {}
        Term(double scalar, std::string name) : m_scalar{scalar}, m_name{std::move(name)} {}
        explicit Term(double scalar) : m_scalar{scalar}, m_name{std::nullopt} {}

        auto scalar() -> double { return m_scalar; }
        auto name() -> std::optional<std::string> & { return m_name; }
    };

    enum class Comparison { GreaterThan, GreaterThenEquals, LessThan, LessThanEquals };

    std::vector<Term> m_left;
    Comparison m_cmp;
    std::vector<Term> m_right;

    Predicate(std::vector<Term> left, Comparison cmp, std::vector<Term> right)
        : m_left{std::move(left)}, m_cmp{cmp}, m_right{std::move(right)} {}
};
}  // namespace forek::ir
