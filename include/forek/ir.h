#pragma once

#include <memory>
#include <string>

namespace forek::ir {
struct True {
    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_true();
    }
};

struct False {
    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_false();
    }
};

struct Proposition {
    std::string m_name;

    Proposition() = delete;
    Proposition(const Proposition& other) = default;
    Proposition(Proposition&& other) = default;

    explicit Proposition(const std::string& name) : m_name{name} {}
    explicit Proposition(std::string&& name) : m_name{std::move(name)} {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_proposition(m_name);
    }
};

template <typename Subtree>
struct Negation {
    std::shared_ptr<Subtree> m_tree;

    Negation() = delete;
    Negation(const Negation<Subtree>& other) = default;
    Negation(Negation<Subtree>&& other) = default;

    explicit Negation(const Subtree& subtree) : m_tree{nullptr} {
        this->m_tree = std::make_shared<Subtree>(subtree);
    }

    explicit Negation(Subtree&& subtree) : m_tree{nullptr} {
        this->m_tree = std::make_shared<Subtree>(std::move(subtree));
    }

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_negation(m_tree->visit(visitor));
    }
};

template <typename Subtree>
struct Conjunction {
    std::shared_ptr<Subtree> m_left;
    std::shared_ptr<Subtree> m_right;

    Conjunction() = delete;
    Conjunction(const Conjunction<Subtree>& other) = default;
    Conjunction(Conjunction<Subtree>&& other) = default;

    Conjunction(const Subtree& left, const Subtree& right) : m_left{nullptr}, m_right{nullptr} {
        this->m_left = std::make_shared<Subtree>(left);
        this->m_right = std::make_shared<Subtree>(right);
    }

    Conjunction(Subtree&& left, Subtree&& right) : m_left{nullptr}, m_right{nullptr} {
        this->m_left = std::make_shared<Subtree>(std::move(left));
        this->m_right = std::make_shared<Subtree>(std::move(right));
    }

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_conjunction(m_left->visit(visitor), m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Disjunction {
    std::shared_ptr<Subtree> m_left;
    std::shared_ptr<Subtree> m_right;

    Disjunction() = delete;
    Disjunction(const Disjunction<Subtree>& other) = default;
    Disjunction(Disjunction<Subtree>&& other) = default;

    Disjunction(const Subtree& left, const Subtree& right) : m_left{nullptr}, m_right{nullptr} {
        this->m_left = std::make_shared<Subtree>(left);
        this->m_right = std::make_shared<Subtree>(right);
    }

    Disjunction(Subtree&& left, Subtree&& right) : m_left{nullptr}, m_right{nullptr} {
        this->m_left = std::make_shared<Subtree>(std::move(left));
        this->m_right = std::make_shared<Subtree>(std::move(right));
    }

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_disjunction(m_left->visit(visitor), m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Implication {
    std::shared_ptr<Subtree> m_left;
    std::shared_ptr<Subtree> m_right;

    Implication() = delete;
    Implication(const Implication<Subtree>& other) = default;
    Implication(Implication<Subtree>&& other) = default;

    Implication(const Subtree& left, const Subtree& right) : m_left{nullptr}, m_right{nullptr} {
        this->m_left = std::make_shared<Subtree>(left);
        this->m_right = std::make_shared<Subtree>(right);
    }

    Implication(Subtree&& left, Subtree&& right) : m_left{nullptr}, m_right{nullptr} {
        this->m_left = std::make_shared<Subtree>(std::move(left));
        this->m_right = std::make_shared<Subtree>(std::move(right));
    }

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_implication(m_left->visit(visitor), m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Equivalence {
    std::shared_ptr<Subtree> m_left;
    std::shared_ptr<Subtree> m_right;

    Equivalence() = delete;
    Equivalence(const Equivalence<Subtree>& other) = default;
    Equivalence(Equivalence<Subtree>&& other) = default;

    Equivalence(const Subtree& left, const Subtree& right) : m_left{nullptr}, m_right{nullptr} {
        this->m_left = std::make_shared<Subtree>(left);
        this->m_right = std::make_shared<Subtree>(right);
    }

    Equivalence(Subtree&& left, Subtree&& right) : m_left{nullptr}, m_right{nullptr} {
        this->m_left = std::make_shared<Subtree>(std::move(left));
        this->m_right = std::make_shared<Subtree>(std::move(right));
    }

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_equivalence(m_left->visit(visitor), m_right->visit(visitor));
    }
};
}  // namespace forek::ir
