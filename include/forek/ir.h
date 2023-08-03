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

template <typename Subtree>
class Unary {
   public:
    Unary() = delete;

   protected:
    std::unique_ptr<Subtree> m_inner;

    Unary(const Unary<Subtree>&) = default;
    Unary(Unary<Subtree>&&) = default;

    explicit Unary(const Subtree& other) : m_inner{nullptr} {
        m_inner = std::make_shared<Subtree>(other);
    }

    explicit Unary(Subtree&& other) : m_inner{nullptr} {
        m_inner = std::make_shared<Subtree>(std::move(other));
    }
};

template <typename Subtree>
struct Globally : public Unary<Subtree> {
    Globally() = delete;
    Globally(const Globally<Subtree>& other) : Unary<Subtree>(other) {}
    Globally(Globally<Subtree>&& other) : Unary<Subtree>(std::move(other)) {}

    explicit Globally(const Subtree& inner) : Unary<Subtree>(inner) {}
    explicit Globally(Subtree&& inner) : Unary<Subtree>(std::move(inner)) {}

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

    explicit Finally(const Subtree& inner) : Unary<Subtree>(inner) {}
    explicit Finally(Subtree&& inner) : Unary<Subtree>(std::move(inner)) {}

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

    explicit Next(const Subtree& inner) : Unary<Subtree>(inner) {}
    explicit Next(Subtree&& inner) : Unary<Subtree>(std::move(inner)) {}

    template<typename V>
    auto visit(V& visitor) {
        return visitor.visit_next(this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
class Binary {
   public:
    Binary() = delete;

   protected:
    std::shared_ptr<Subtree> m_left;
    std::shared_ptr<Subtree> m_right;

    Binary(const Binary<Subtree>&) = default;
    Binary(Binary<Subtree>&&) = default;

    Binary(const Subtree& left, const Subtree& right) : m_left{nullptr}, m_right{nullptr} {
        m_left = std::make_shared<Subtree>(left);
        m_right = std::make_shared<Subtree>(right);
    }

    Binary(const Subtree& left, Subtree&& right) : m_left{nullptr}, m_right{nullptr} {
        m_left = std::make_shared<Subtree>(left);
        m_right = std::make_shared<Subtree>(std::move(right));
    }

    Binary(Subtree&& left, const Subtree& right) : m_left{nullptr}, m_right{nullptr} {
        m_left = std::make_shared<Subtree>(std::move(left));
        m_right = std::make_shared<Subtree>(right);
    }

    Binary(Subtree&& left, Subtree&& right) : m_left{nullptr}, m_right{nullptr} {
        m_left = std::make_shared<Subtree>(std::move(left));
        m_right = std::make_shared<Subtree>(std::move(right));
    }
};

template <typename Subtree>
class Release : Binary<Subtree> {
   public:
    Release() = delete;
    Release(const Release<Subtree>& other) : Binary<Subtree>(other) {}
    Release(Release<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}

    Release(const Subtree& left, const Subtree& right) : Binary<Subtree>(left, right) {}
    Release(const Subtree& left, Subtree&& right) : Binary<Subtree>(left, std::move(right)) {}
    Release(Subtree&& left, const Subtree& right) : Binary<Subtree>(std::move(left), right) {}
    Release(Subtree&& left, Subtree&& right) : Binary<Subtree>(std::move(left), std::move(right)) {}

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

    Until(const Subtree& left, const Subtree& right) : Binary<Subtree>(left, right) {}
    Until(const Subtree& left, Subtree&& right) : Binary<Subtree>(left, std::move(right)) {}
    Until(Subtree&& left, const Subtree& right) : Binary<Subtree>(std::move(left), right) {}
    Until(Subtree&& left, Subtree&& right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_until(this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};
}  // namespace forek::ir
