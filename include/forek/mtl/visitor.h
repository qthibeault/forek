#pragma once

#include "forek/interval.h"
#include "forek/ltl/visitor.h"

namespace forek::mtl {
template <typename T>
class Visitor : public virtual ltl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
    virtual auto visit_bounded_next(const interval::Interval &, T) -> T = 0;
    virtual auto visit_bounded_globally(const interval::Interval &, T) -> T = 0;
    virtual auto visit_bounded_finally(const interval::Interval &, T) -> T = 0;
    virtual auto visit_bounded_release(const interval::Interval &, T) -> T = 0;
    virtual auto visit_bounded_until(const interval::Interval &, T) -> T = 0;
};
}  // namespace forek::mtl
