#pragma once

#include <memory>
#include <variant>

#include "forek/ir.h"
#include "forek/ltl/visitor.h"

namespace forek::ltl {
class Tree {
   private:
    using Node = std::variant<ir::True, ir::False, ir::Proposition, ir::Negation<Tree>,
                              ir::Conjunction<Tree>, ir::Disjunction<Tree>, ir::Implication<Tree>,
                              ir::Equivalence<Tree>, ir::Globally<Tree>, ir::Finally<Tree>,
                              ir::Next<Tree>, ir::Release<Tree>, ir::Until<Tree> >;

    Node m_node;

   public:
    Tree() = delete;
    Tree(const Tree& other) = default;
    Tree(Tree&& other) = default;

    explicit Tree(const Node& node) : m_node{node} {}
    explicit Tree(Node&& node) : m_node{std::move(node)} {}

    template <typename T>
    auto visit(Visitor<T>& v) {
        return std::visit([&v](const auto& node) { return node.visit(v); }, m_node);
    }
};
}  // namespace forek::ltl
