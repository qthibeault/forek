#pragma once

#include "forek/interval.h"
#include "forek/ltl/visitor.h"

namespace forek::mtl {

template <typename T>
class Visitor : public ltl::Visitor<T> {
   private:
    using Interval = interval::Interval;

   public:
    Visitor() = default;
    Visitor(const Visitor<T>&) = default;
    Visitor(Visitor<T>&&) = default;

    auto operator=(const Visitor<T>&) -> Visitor<T>& = default;
    auto operator=(Visitor<T>&&) -> Visitor<T>& = default;

    virtual ~Visitor() = default;
    virtual auto visit_bounded_next(const Interval &, T) -> T = 0;
    virtual auto visit_bounded_globally(const Interval &, T) -> T = 0;
    virtual auto visit_bounded_finally(const Interval &, T) -> T = 0;
    virtual auto visit_bounded_release(const Interval &, T, T) -> T = 0;
    virtual auto visit_bounded_until(const Interval &, T, T) -> T = 0;
};

}  // namespace forek::mtl
