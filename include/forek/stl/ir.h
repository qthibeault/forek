#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "forek/algebra.h"
#include "forek/mtl/ir.h"


namespace forek::ir {
struct Predicate {
    using Expr = algebra::Expr;
    using Comparison = algebra::Comparison;

    Expr m_left;
    Comparison m_cmp;
    Expr m_right;

    Predicate(Expr left, Comparison cmp, Expr right)
        : m_left{std::move(left)}, m_cmp{cmp}, m_right{std::move(right)} {}

    template <typename V>
    auto accept(V& visitor) const -> decltype(auto) {
        visitor.visit_predicate(m_left, m_cmp, m_right);
    }
};
}  // namespace forek::ir
