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

    explicit Proposition(std::string name) : m_name{std::move(name)} {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_proposition(m_name);
    }
};

template <typename Subtree>
class Unary {
   public:
   protected:
    std::shared_ptr<Subtree> m_inner{nullptr};

    explicit Unary(Subtree&& tree) { m_inner = std::make_shared<Subtree>(std::move(tree)); }
};

template <typename Subtree>
struct Negation : Unary<Subtree> {
    explicit Negation(Subtree inner) : Unary<Subtree>(std::move(inner)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_negation(this->m_tree->visit(visitor));
    }
};

template <typename Subtree>
class Binary {
   public:
   protected:
    std::shared_ptr<Subtree> m_left{nullptr};
    std::shared_ptr<Subtree> m_right{nullptr};

    Binary(Subtree&& left, Subtree&& right) {
        m_left = std::make_shared<Subtree>(std::move(left));
        m_right = std::make_shared<Subtree>(std::move(right));
    }
};

template <typename Subtree>
struct Conjunction : Binary<Subtree> {
    Conjunction(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_conjunction(this->m_left->visit(visitor),
                                         this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Disjunction : Binary<Subtree> {
    Disjunction(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_disjunction(this->m_left->visit(visitor),
                                         this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Implication : Binary<Subtree> {
    Implication(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_implication(this->m_left->visit(visitor),
                                         this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Equivalence : Binary<Subtree> {
    Equivalence(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_equivalence(this->m_left->visit(visitor),
                                         this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Globally : public Unary<Subtree> {
    explicit Globally(Subtree inner) : Unary<Subtree>(std::move(inner)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_globally(this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
struct Finally : public Unary<Subtree> {
    explicit Finally(Subtree inner) : Unary<Subtree>(std::move(inner)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_finally(this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
struct Next : public Unary<Subtree> {
    explicit Next(Subtree inner) : Unary<Subtree>(std::move(inner)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_next(this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
struct Release : Binary<Subtree> {
    Release(Subtree left, Subtree&& right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_release(this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct Until : Binary<Subtree> {
    Until(Subtree left, Subtree right) : Binary<Subtree>(std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_until(this->m_left->visit(visitor), this->m_right->visit(visitor));
    }
};

template <typename Subtree>
class BoundedUnary : public Unary<Subtree> {
   protected:
    interval::Interval m_interval;

    BoundedUnary(interval::Interval&& i, Subtree&& tree)
        : m_interval{std::move(i)}, Unary<Subtree>(tree) {}
};

template <typename Subtree>
struct BoundedGlobally : public BoundedUnary<Subtree> {
    BoundedGlobally(interval::Interval i, Subtree tree)
        : BoundedUnary<Subtree>(std::move(i), std::move(tree)) {}

    template <typename V>
    auto visit(V& visitor) {
        return this->m_inner.visit_bounded_globally(this->m_interval,
                                                    this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
struct BoundedFinally : public BoundedUnary<Subtree> {
    BoundedFinally(interval::Interval i, Subtree tree)
        : BoundedUnary<Subtree>(std::move(i), std::move(tree)) {}

    template <typename V>
    auto visit(V& visitor) {
        return this->m_inner.visit_bounded_finally(this->m_interval, this->m_inner->visit(visitor));
    }
};

template <typename Subtree>
class BoundedBinary : public Binary<Subtree> {
   protected:
    interval::Interval m_interval;

    BoundedBinary(const BoundedBinary<Subtree>& other) : Binary<Subtree>(other) {}
    BoundedBinary(BoundedBinary<Subtree>&& other) : Binary<Subtree>(std::move(other)) {}
    BoundedBinary(interval::Interval&& i, Subtree&& left, Subtree&& right)
        : m_interval{std::move(i)}, Binary<Subtree>(std::move(left), std::move(right)) {}
};

template <typename Subtree>
struct BoundedRelease : public BoundedBinary<Subtree> {
    BoundedRelease(interval::Interval i, Subtree left, Subtree right)
        : BoundedBinary<Subtree>(std::move(i), std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_bounded_release(this->m_interval, this->m_left->visit(visitor),
                                             this->m_right->visit(visitor));
    }
};

template <typename Subtree>
struct BoundedUntil : public BoundedBinary<Subtree> {
    BoundedUntil(interval::Interval i, Subtree left, Subtree right)
        : BoundedBinary<Subtree>(std::move(i), std::move(left), std::move(right)) {}

    template <typename V>
    auto visit(V& visitor) {
        return visitor.visit_bounded_until(this->m_interval, this->m_left->visit(visitor),
                                           this->m_right->visit(visitor));
    }
};
}  // namespace forek::ir
