#pragma once

#include "forek/pl/visitor.h"

namespace forek::ltl {

template <typename T>
class Visitor : public pl::Visitor<T> {
   public:
    Visitor() = default;
    Visitor(const Visitor<T>&) = default;
    Visitor(Visitor<T>&&) = default;

    auto operator=(const Visitor<T>&) -> Visitor<T>& = default;
    auto operator=(Visitor<T>&&) -> Visitor<T>& = default;

    virtual ~Visitor() = default;
    virtual auto visit_globally(T inner) -> T = 0;
    virtual auto visit_finally(T inner) -> T = 0;
    virtual auto visit_next(T inner) -> T = 0;
    virtual auto visit_release(T left, T right) -> T = 0;
    virtual auto visit_until(T left, T right) -> T = 0;
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
