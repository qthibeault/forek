#pragma once

#include <variant>

#include "forek/stl/ir.h"
#include "forek/stl/visitor.h"

namespace forek::stl {

class Tree {
    using Node =
        std::variant<ir::True, ir::False, ir::Proposition, ir::Predicate, ir::Negation<Tree>,
                     ir::Conjunction<Tree>, ir::Disjunction<Tree>, ir::Implication<Tree>,
                     ir::Equivalence<Tree>, ir::Globally<Tree>, ir::Finally<Tree>, ir::Next<Tree>,
                     ir::Release<Tree>, ir::Until<Tree>, ir::BoundedGlobally<Tree>,
                     ir::BoundedFinally<Tree>, ir::BoundedNext<Tree>, ir::BoundedRelease<Tree>,
                     ir::BoundedUntil<Tree>>;

    Node m_node;

   public:
    explicit Tree(Node node) : m_node(std::move(node)) {}

    template <typename T>
    auto accept(Visitor<T>& v) -> T {
        return std::visit([&v](const auto& node) { return node.accept(v); }, m_node);
    }
};

}  // namespace forek::stl
