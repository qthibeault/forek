#pragma once

#include <memory>
#include <string>

namespace forek::ir {
struct True {
    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_true();
    }
};

struct False {
    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_false();
    }
};

struct Proposition {
    std::string m_name;

    explicit Proposition(std::string name) : m_name{std::move(name)} {}

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_proposition(m_name);
    }
};

template <typename Subtree>
struct Unary {
    std::shared_ptr<Subtree> m_inner{nullptr};

    explicit Unary(Subtree inner) : m_inner{std::make_shared<Subtree>(std::move(inner))} {}
    explicit Unary(std::shared_ptr<Subtree> inner) : m_inner{std::move(inner)} {}
};

template <typename Subtree>
struct Negation : public Unary<Subtree> {
    using Unary<Subtree>::Unary;

    template <typename V>
    inline auto accept(V &visitor) const {
        return visitor.visit_negation(this->m_tree->accept(visitor));
    }
};

template <typename Subtree>
struct Binary {
    std::shared_ptr<Subtree> m_left{nullptr};
    std::shared_ptr<Subtree> m_right{nullptr};

    Binary(Subtree left, Subtree right)
        : m_left{std::make_shared<Subtree>(std::move(left))},
          m_right{std::make_shared<Subtree>(std::move(right))} {}

    Binary(std::shared_ptr<Subtree> left, std::shared_ptr<Subtree> right)
        : m_left{std::move(left)}, m_right{std::move(right)} {}
};

template <typename Subtree>
struct Conjunction : public Binary<Subtree> {
    using Binary<Subtree>::Binary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_conjunction(this->m_left->accept(visitor),
                                         this->m_right->accept(visitor));
    }
};

template <typename Subtree>
struct Disjunction : public Binary<Subtree> {
    using Binary<Subtree>::Binary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_disjunction(this->m_left->accept(visitor),
                                         this->m_right->accept(visitor));
    }
};

template <typename Subtree>
struct Implication : public Binary<Subtree> {
    using Binary<Subtree>::Binary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_implication(this->m_left->accept(visitor),
                                         this->m_right->accept(visitor));
    }
};

template <typename Subtree>
struct Equivalence : public Binary<Subtree> {
    using Binary<Subtree>::Binary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_equivalence(this->m_left->accept(visitor),
                                         this->m_right->accept(visitor));
    }
};
}  // namespace forek::ir
