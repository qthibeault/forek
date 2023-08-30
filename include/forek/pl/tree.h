#pragma once

#include <utility>
#include <variant>

#include "forek/pl/ir.h"
#include "forek/pl/visitor.h"

namespace forek::pl {
class Tree {
   private:
    using Node = std::variant<forek::ir::True, forek::ir::False, forek::ir::Proposition,
                              forek::ir::Negation<Tree>, forek::ir::Conjunction<Tree>,
                              forek::ir::Disjunction<Tree>, forek::ir::Implication<Tree>,
                              forek::ir::Equivalence<Tree>>;

    Node m_node;

   public:
    explicit Tree(Node node) : m_node{std::move(node)} {}

    auto operator==(const Tree &tree) const -> bool;

    template <typename T>
    auto accept(forek::pl::Visitor<T> &visitor) -> T {
        return std::visit([&visitor](const auto &node) { return node.accept(visitor); }, m_node);
    }
};
}  // namespace forek::pl
