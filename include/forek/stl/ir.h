#pragma once

#include <optional>
#include <string>
#include <vector>

#include "forek/algebra.h"
#include "forek/mtl/ir.h"

namespace forek::ir {
struct Predicate {
    using Sum = algebra::Sum;
    using Comparison = algebra::Comparison;

    Sum m_left;
    Comparison m_cmp;
    Sum m_right;

    Predicate(Sum left, Comparison cmp, Sum right)
        : m_left{std::move(left)}, m_cmp{cmp}, m_right{std::move(right)} {}

    template <typename V>
    auto accept(V& visitor) const -> decltype(auto) {
        visitor.visit_predicate(m_left.terms(), m_cmp, m_right.terms());
    }
};
}  // namespace forek::ir
