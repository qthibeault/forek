#pragma once

#include <string>

namespace forek::pl {

template <typename T>
class Visitor {
   public:
    Visitor() = default;
    Visitor(const Visitor&) = default;
    Visitor(Visitor&&) = default;

    auto operator=(const Visitor&) -> Visitor& = default;
    auto operator=(Visitor&&) -> Visitor& = default;

    virtual auto visit_true() -> T = 0;
    virtual auto visit_false() -> T = 0;
    virtual auto visit_proposition(std::string name) -> T = 0;
    virtual auto visit_negation(T inner) -> T = 0;
    virtual auto visit_conjunction(T left, T right) -> T = 0;
    virtual auto visit_disjunction(T left, T right) -> T = 0;
    virtual auto visit_implication(T ante, T cons) -> T = 0;
    virtual auto visit_equivalence(T ante, T cons) -> T = 0;
    virtual ~Visitor() = default;
};

}  // namespace forek::pl
