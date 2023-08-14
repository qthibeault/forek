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

    explicit Proposition(std::string name) : m_name{std::move(name)} {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_proposition(m_name);
    }
};

template <typename Subtree>
class Unary {
   public:
    Unary() = delete;

   protected:
    std::shared_ptr<Subtree> m_inner {nullptr};

    Unary(const Unary<Subtree>&) = default;
    Unary(Unary<Subtree>&&) = default;

    explicit Unary(Subtree&& tree) {
        m_inner = std::make_shared<Subtree>(std::move(tree));
    }
};

template <typename Subtree>
struct Negation : Unary<Subtree> {
    Negation() = delete;
    Negation(const Negation<Subtree>& other) : Unary<Subtree>(other) {}
    Negation(Negation<Subtree>&& other) : Unary<Subtree>(std::move(other)) {}

    explicit Negation(Subtree inner) : Unary<Subtree>(std::move(inner)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_negation(this->m_tree->visit(visitor));
    }
};

template <typename Subtree>
class Binary {
   public:
    Binary() = delete;

   protected:
    std::shared_ptr<Subtree> m_left {nullptr};
    std::shared_ptr<Subtree> m_right {nullptr};

    Binary(const Binary<Subtree>&) = default;
    Binary(Binary<Subtree>&&) = default;

    Binary(Subtree&& left, Subtree&& right) {
        m_left = std::make_shared<Subtree>(std::move(left));
        m_right = std::make_shared<Subtree>(std::move(right));
    }
};

template <typename Subtree>
struct Conjunction : Binary<Subtree> {
    Conjunction() = delete;
    Conjunction(const Conjunction<Subtree>& other) : Binary<Subtree>(other) {}
    Conjunction(Conjunction<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}
    Conjunction(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_conjunction(this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Disjunction : Binary<Subtree> {
    Disjunction() = delete;
    Disjunction(const Disjunction<Subtree>& other) : Binary<Subtree>(other) {}
    Disjunction(Disjunction<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}
    Disjunction(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_disjunction(this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Implication : Binary<Subtree> {
    Implication() = delete;
    Implication(const Implication<Subtree>& other) : Binary<Subtree>(other) {}
    Implication(Implication<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}
    Implication(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_implication(this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Equivalence : Binary<Subtree> {
    Equivalence() = delete;
    Equivalence(const Equivalence<Subtree>& other) : Binary<Subtree>(other) {};
    Equivalence(Equivalence<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}
    Equivalence(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_equivalence(this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Globally : public Unary<Subtree> {
    Globally() = delete;
    Globally(const Globally<Subtree>& other) : Unary<Subtree>(other) {}
    Globally(Globally<Subtree>&& other) : Unary<Subtree>(std::move(other)) {}

    explicit Globally(Subtree inner) : Unary<Subtree>(std::move(inner)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_globally(this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
struct Finally : public Unary<Subtree> {
    Finally() = delete;
    Finally(const Finally<Subtree>& other) : Unary<Subtree>(other) {}
    Finally(Finally<Subtree>&& other) : Unary<Subtree>(std::move(other)) {}

    explicit Finally(Subtree inner) : Unary<Subtree>(std::move(inner)) {}

    template<typename V>
    auto visit(V& visitor) {
        return visitor.visit_finally(this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
struct Next : public Unary<Subtree> {
    Next() = delete;
    Next(const Next<Subtree>& other) : Unary<Subtree>(other) {}
    Next(Next<Subtree>&& other) : Unary<Subtree>(std::move(other)) {}

    explicit Next(Subtree inner) : Unary<Subtree>(std::move(inner)) {}

    template<typename V>
    auto visit(V& visitor) {
        return visitor.visit_next(this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
class Release : Binary<Subtree> {
   public:
    Release() = delete;
    Release(const Release<Subtree>& other) : Binary<Subtree>(other) {}
    Release(Release<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}

    Release(Subtree left, Subtree&& right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_release(this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};

template <typename Subtree>
class Until : Binary<Subtree> {
   public:
    Until() = delete;
    Until(const Until<Subtree>& other) : Binary<Subtree>(other) {}
    Until(Until<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}

    Until(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_until(this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};
}  // namespace forek::ir
