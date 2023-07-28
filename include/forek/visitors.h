#pragma once

#include <string>

namespace forek {
template <typename T>
class Visitor {
    public:
        virtual ~Visitor() = default;
};

namespace pl {
template <typename T>
class Visitor : public virtual forek::Visitor<T> {
   public:
    virtual auto visit_true() -> T;
    virtual auto visit_false() -> T;
    virtual auto visit_proposition(std::string name) -> T;
    virtual auto visit_negation(T inner) -> T;
    virtual auto visit_conjunction(T left, T right) -> T;
    virtual auto visit_disjunction(T left, T right) -> T;
    virtual auto visit_implication(T ante, T cons) -> T;
    virtual auto visit_equivalence(T ante, T cons) -> T;
};
}  // namespace pl

namespace ltl {
template <typename T>
class Visitor : public virtual pl::Visitor<T> {
   public:
    virtual auto visit_always(T inner) -> T;
    virtual auto visit_eventually(T inner) -> T;
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
}  // namespace ltl

namespace mtl {
template <typename T>
class Visitor : public virtual ltl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
};
}  // namespace mtl

namespace stl {
template <typename T>
class Visitor : public virtual mtl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
};
}  // namespace stl

namespace tptl {
template <typename T>
class Visitor : public virtual ltl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
};
}  // namespace tptl

namespace tqtl {
template <typename T>
class Visitor : public virtual tptl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
};
}  // namespace tqtl

namespace stpl {
template <typename T>
class Visitor : public virtual tqtl::Visitor<T> {
   public:
    virtual ~Visitor() = default;
};
}  // namespace stpl
}  // namespace forek
