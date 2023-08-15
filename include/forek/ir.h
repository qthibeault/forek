#pragma once

#include <memory>
#include <string>

#include "forek/interval.h"

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
    std::shared_ptr<Subtree> m_inner{nullptr};

    Unary(const Unary<Subtree>&) = default;
    Unary(Unary<Subtree>&&) = default;

    explicit Unary(Subtree&& tree) { m_inner = std::make_shared<Subtree>(std::move(tree)); }
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
    std::shared_ptr<Subtree> m_left{nullptr};
    std::shared_ptr<Subtree> m_right{nullptr};

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
        return visitor.visit_conjunction(this->m_left->visit(visitor),
                                         this->m_right->visit(visitor));
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
        return visitor.visit_disjunction(this->m_left->visit(visitor),
                                         this->m_right->visit(visitor));
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
        return visitor.visit_implication(this->m_left->visit(visitor),
                                         this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Equivalence : Binary<Subtree> {
    Equivalence() = delete;
    Equivalence(const Equivalence<Subtree>& other) : Binary<Subtree>(other){};
    Equivalence(Equivalence<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}
    Equivalence(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_equivalence(this->m_left->visit(visitor),
                                         this->m_right->visit(visitor));
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

    template <typename V>
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

    template <typename V>
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

template <typename Subtree>
class BoundedUnary : public Unary<Subtree> {
   public:
    BoundedUnary() = delete;

   protected:
    interval::Interval m_interval;

    BoundedUnary(const BoundedUnary<Subtree>& other) : Unary<Subtree>(other) {}
    BoundedUnary(BoundedUnary<Subtree>&& other) : Unary<Subtree>(std::move(other)) {}
    BoundedUnary(interval::Interval&& i, Subtree&& tree)
        : m_interval{std::move(i)}, Unary<Subtree>(tree) {}
};

template <typename Subtree>
class BoundedGlobally : public BoundedUnary<Subtree> {
   public:
    BoundedGlobally() = delete;
    BoundedGlobally(const BoundedGlobally<Subtree>& other) : BoundedUnary<Subtree>(other) {}
    BoundedGlobally(BoundedGlobally<Subtree>&& other) : BoundedUnary<Subtree>(std::move(other)) {}
    BoundedGlobally(interval::Interval i, Subtree tree)
        : BoundedUnary<Subtree>(std::move(i), std::move(tree)) {}

    template <typename V>
    auto visit(V& visitor) {
        return this->m_inner.visit_bounded_globally(this->m_interval, this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
class BoundedFinally : public BoundedUnary<Subtree> {
   public:
    BoundedFinally() = delete;
    BoundedFinally(const BoundedFinally<Subtree>& other) : BoundedUnary<Subtree>(other) {}
    BoundedFinally(BoundedFinally<Subtree>&& other) : BoundedUnary<Subtree>(std::move(other)) {}
    BoundedFinally(interval::Interval i, Subtree tree)
        : BoundedUnary<Subtree>(std::move(i), std::move(tree)) {}

    template <typename V>
    auto visit(V& visitor) {
        return this->m_inner.visit_bounded_finally(this->m_interval, this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
class BoundedBinary : public Binary<Subtree> {
   public:
    BoundedBinary() = delete;

   private:
    interval::Interval m_interval;

    BoundedBinary(const BoundedBinary<Subtree>& other) : Binary<Subtree>(other) {}
    BoundedBinary(BoundedBinary<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}
    BoundedBinary(interval::Interval&& i, Subtree&& left, Subtree&& right)
        : m_interval{std::move(i)}, Binary<Subtree>(std::move(left), std::move(right)) {}
};

template <typename Subtree>
class BoundedRelease : public BoundedBinary<Subtree> {
   public:
    BoundedRelease() = delete;
    BoundedRelease(const BoundedRelease<Subtree>& other) : BoundedBinary<Subtree>(other) {}
    BoundedRelease(BoundedRelease<Subtree>&& other) : BoundedBinary<Subtree>(std::move(other)) {}
    BoundedRelease(interval::Interval i, Subtree left, Subtree right) : BoundedBinary<Subtree>(std::move(i), std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_bounded_release(this->m_interval, this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};

template <typename Subtree>
class BoundedUntil : public BoundedBinary<Subtree> {
   public:
    BoundedUntil() = delete;
    BoundedUntil(const BoundedUntil<Subtree>& other) : BoundedBinary<Subtree>(other) {}
    BoundedUntil(BoundedUntil<Subtree>&& other) : BoundedBinary<Subtree>(std::move(other)) {}
    BoundedUntil(interval::Interval i, Subtree left, Subtree right) : BoundedBinary<Subtree>(std::move(i), std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_bounded_until(this->m_interval, this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};
}  // namespace forek::ir
