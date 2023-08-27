#pragma once

#include <variant>

#include "forek/mtl/ir.h"
#include "forek/mtl/visitor.h"

namespace forek::mtl {
class Tree {
   private:
    using Node =
        std::variant<ir::True, ir::False, ir::Proposition, ir::Negation<Tree>,
                     ir::Conjunction<Tree>, ir::Disjunction<Tree>, ir::Implication<Tree>,
                     ir::Equivalence<Tree>, ir::Globally<Tree>, ir::Finally<Tree>, ir::Next<Tree>,
                     ir::Release<Tree>, ir::Until<Tree>, ir::BoundedGlobally<Tree>,
                     ir::BoundedFinally<Tree>, ir::BoundedNext<Tree>, ir::BoundedRelease<Tree>,
                     ir::BoundedUntil<Tree>>;

    Node m_inner;

   public:
    explicit Tree(Node node) : m_inner{std::move(node)} {}

    template <typename T>
    auto accept(Visitor<T>& v) -> T {
        std::visit([&v](const auto& inner) { return inner.accept(v); }, m_inner);
    }
};
}  // namespace forek::mtl
