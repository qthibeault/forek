#pragma once

#include <variant>

#include "forek/ir.h"
#include "forek/pl/visitor.h"

namespace forek::pl {
class Tree {
   private:
    using Node = std::variant<forek::ir::True, forek::ir::False, forek::ir::Proposition,
                              forek::ir::Negation<Tree>, forek::ir::Conjunction<Tree>,
                              forek::ir::Disjunction<Tree>, forek::ir::Implication<Tree>,
                              forek::ir::Equivalence<Tree> >;

    Node m_node;

   public:
    Tree() = delete;
    Tree(const Tree& other) = default;

    explicit Tree(const Node& node) : m_node{node} {}
    explicit Tree(Node&& node) : m_node{std::move(node)} {}

    template <typename T>
    auto visit(forek::pl::Visitor<T>& visitor) -> T {
        return std::visit([&visitor](const auto& node) { return node.visit(visitor); }, m_node);
    }
};
}  // namespace forek::pl
