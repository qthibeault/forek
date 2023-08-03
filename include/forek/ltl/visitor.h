#pragma once

#include "forek/pl/visitor.h"

namespace forek::ltl {
template <typename T>
class Visitor : public virtual pl::Visitor<T> {
   public:
    virtual auto visit_globally(T inner) -> T;
    virtual auto visit_finally(T inner) -> T;
    virtual auto visit_next(T inner) -> T;
    virtual auto visit_release(T inner) -> T;
    virtual auto visit_until(T inner) -> T;
};

template <typename T>
class PastTimeVisitor : public virtual Visitor<T> {
   public:
    virtual auto visit_historically(T inner) -> T;
    virtual auto visit_once(T inner) -> T;
    virtual auto visit_previous(T inner) -> T;
    virtual auto visit_trigger(T inner) -> T;
    virtual auto visit_since(T inner) -> T;
};
}  // namespace forek::ltl
