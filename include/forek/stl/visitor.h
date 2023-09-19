#pragma once

#include <span>

#include "forek/algebra.h"
#include "forek/mtl/visitor.h"

namespace forek::stl {

template <typename T>
class Visitor : public virtual mtl::Visitor<T> {
   public:
    using Expr = algebra::Expr;
    using Comparison = algebra::Comparison;

    Visitor() = default;
    Visitor(const Visitor<T>&) = default;
    Visitor(Visitor<T>&&) = default;

    auto operator=(const Visitor<T>&) -> Visitor<T>& = default;
    auto operator=(Visitor<T>&&) -> Visitor<T>& = default;

    virtual ~Visitor() = default;
    virtual auto visit_predicate(const Expr& lhs, Comparison cmp, const Expr& rhs) -> T = 0;
};

}  // namespace forek::stl
