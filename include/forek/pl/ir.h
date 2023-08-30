#pragma once

#include <memory>
#include <string>

namespace forek::ir {
struct True {
    auto operator==(const True &) const -> bool { return true; }

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_true();
    }
};

struct False {
    auto operator==(const False &) const -> bool { return true; }

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_false();
    }
};

struct Proposition {
    std::string m_name;

    explicit Proposition(std::string name) : m_name{std::move(name)} {}

    auto operator==(const Proposition &other) const -> bool { return m_name == other.m_name; }

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

    auto operator==(const Unary<Subtree> &other) const -> bool { return *m_inner == *other.m_inner; }
};

template <typename Subtree>
struct Negation : public Unary<Subtree> {
    using Unary<Subtree>::Unary;
    using Unary<Subtree>::operator==;

    template <typename V>
    inline auto accept(V &visitor) const {
        return visitor.visit_negation(this->m_inner->accept(visitor));
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

    auto operator==(const Binary<Subtree> &other) const -> bool {
        return (*m_left == *other.m_left) && (*m_right == *other.m_right);
    }
};

template <typename Subtree>
struct Conjunction : public Binary<Subtree> {
    using Binary<Subtree>::Binary;
    using Binary<Subtree>::operator==;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_conjunction(this->m_left->accept(visitor),
                                         this->m_right->accept(visitor));
    }
};

template <typename Subtree>
struct Disjunction : public Binary<Subtree> {
    using Binary<Subtree>::Binary;
    using Binary<Subtree>::operator==;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_disjunction(this->m_left->accept(visitor),
                                         this->m_right->accept(visitor));
    }
};

template <typename Subtree>
struct Implication : public Binary<Subtree> {
    using Binary<Subtree>::Binary;
    using Binary<Subtree>::operator==;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_implication(this->m_left->accept(visitor),
                                         this->m_right->accept(visitor));
    }
};

template <typename Subtree>
struct Equivalence : public Binary<Subtree> {
    using Binary<Subtree>::Binary;
    using Binary<Subtree>::operator==;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_equivalence(this->m_left->accept(visitor),
                                         this->m_right->accept(visitor));
    }
};
}  // namespace forek::ir
