#pragma once

#include <span>

#include "forek/mtl/visitor.h"

namespace forek::stl {
template <typename T>
class Visitor : public virtual mtl::Visitor<T> {
   public:
    Visitor() = default;
    Visitor(const Visitor&) = default;
    Visitor(Visitor&&) = default;

    auto operator=(const Visitor&) -> Visitor& = default;
    auto operator=(Visitor&&) -> Visitor& = default;

    auto visit_predicate() -> T = 0;

    virtual ~Visitor() = default;
};
}  // namespace forek::stl
