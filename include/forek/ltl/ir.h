#pragma once

#include "forek/pl/ir.h"

namespace forek::ir {
template <typename Subtree>
struct Globally : public Unary<Subtree> {
    using Unary<Subtree>::Unary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_globally(this->m_inner->accept(visitor));
    }
};

template <typename Subtree>
struct Finally : public Unary<Subtree> {
    using Unary<Subtree>::Unary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_finally(this->m_inner->accept(visitor));
    }
};

template <typename Subtree>
struct Next : public Unary<Subtree> {
    using Unary<Subtree>::Unary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_next(this->m_inner->accept(visitor));
    }
};

template <typename Subtree>
struct Release : public Binary<Subtree> {
    using Binary<Subtree>::Binary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_release(this->m_left->accept(visitor), this->m_right->accept(visitor));
    }
};

template <typename Subtree>
struct Until : public Binary<Subtree> {
    using Binary<Subtree>::Binary;

    template <typename V>
    auto accept(V &visitor) const {
        return visitor.visit_until(this->m_left->accept(visitor), this->m_right->accept(visitor));
    }
};
}  // namespace forek::ir
