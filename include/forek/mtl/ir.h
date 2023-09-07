#pragma once

#include "forek/interval.h"
#include "forek/ltl/ir.h"

namespace forek::ir {
template <typename Subtree>
class BoundedUnary : public Unary<Subtree> {
    using Interval = interval::Interval;

   public:
    Interval m_interval;

    BoundedUnary(Interval i, Subtree inner)
        : Unary<Subtree>(std::move(inner)), m_interval{std::move(i)} {}

    BoundedUnary(Interval i, std::shared_ptr<Subtree> inner)
        : Unary<Subtree>(std::move(inner)), m_interval{std::move(i)} {}

    auto operator==(const BoundedUnary<Subtree> &lhs) const -> bool {
        return this->m_interval == lhs.m_interval && Unary<Subtree>::operator==(lhs);
    }
};

template <typename Subtree>
struct BoundedNext : public BoundedUnary<Subtree> {
    using BoundedUnary<Subtree>::BoundedUnary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_bounded_next(this->m_interval, this->m_inner->accept(visitor));
    }
};

template <typename Subtree>
struct BoundedGlobally : public BoundedUnary<Subtree> {
    using BoundedUnary<Subtree>::BoundedUnary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_bounded_globally(this->m_interval, this->m_inner->accept(visitor));
    }
};

template <typename Subtree>
struct BoundedFinally : public BoundedUnary<Subtree> {
    using BoundedUnary<Subtree>::BoundedUnary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_bounded_finally(this->m_interval, this->m_inner->accept(visitor));
    }
};

template <typename Subtree>
class BoundedBinary : public Binary<Subtree> {
    using Interval = interval::Interval;

   public:
    Interval m_interval;

    BoundedBinary(Interval i, Subtree left, Subtree right)
        : Binary<Subtree>(std::move(left), std::move(right)), m_interval{std::move(i)} {}

    BoundedBinary(Interval i, std::shared_ptr<Subtree> left, std::shared_ptr<Subtree> right)
        : Binary<Subtree>(std::move(left), std::move(right)), m_interval{std::move(i)} {}

    auto operator==(const BoundedBinary<Subtree> &lhs) const -> bool {
        return this->m_interval == lhs.m_interval && Binary<Subtree>::operator==(lhs);
    }
};

template <typename Subtree>
struct BoundedRelease : public BoundedBinary<Subtree> {
    using BoundedBinary<Subtree>::BoundedBinary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_bounded_release(this->m_interval, this->m_left->accept(visitor),
                                             this->m_right->accept(visitor));
    }
};

template <typename Subtree>
struct BoundedUntil : public BoundedBinary<Subtree> {
    using BoundedBinary<Subtree>::BoundedBinary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_bounded_until(this->m_interval, this->m_left->accept(visitor),
                                           this->m_right->accept(visitor));
    }
};
}  // namespace forek::ir
