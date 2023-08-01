#pragma once

#include <string>

namespace forek::pl {
template <typename T>
class Visitor {
   public:
    virtual auto visit_true() -> T;
    virtual auto visit_false() -> T;
    virtual auto visit_proposition(std::string name) -> T;
    virtual auto visit_negation(T inner) -> T;
    virtual auto visit_conjunction(T left, T right) -> T;
    virtual auto visit_disjunction(T left, T right) -> T;
    virtual auto visit_implication(T ante, T cons) -> T;
    virtual auto visit_equivalence(T ante, T cons) -> T;
    virtual ~Visitor() = default;
};
}  // namespace forek::pl
