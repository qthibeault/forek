#pragma once

#include <span>

#include "forek/algebra.h"
#include "forek/mtl/visitor.h"

namespace forek::stl {
template <typename T>
class Visitor : public virtual mtl::Visitor<T> {
    using Expr = algebra::Expr;
    using Comparison = algebra::Comparison;

   public:
    Visitor() = default;
    Visitor(const Visitor&) = default;
    Visitor(Visitor&&) = default;

    auto operator=(const Visitor&) -> Visitor& = default;
    auto operator=(Visitor&&) -> Visitor& = default;

    auto visit_predicate(const Expr& lhs, Comparison cmp, const Expr& rhs) -> T = 0;

    virtual ~Visitor() = default;
};
}  // namespace forek::stl
