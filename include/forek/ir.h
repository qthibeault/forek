#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "forek/interval.h"

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

struct Predicate {
    class Term {
        double m_scalar;
        std::optional<std::string> m_name;

       public:
        Term(std::string name, double scalar) : m_scalar{scalar}, m_name{std::move(name)} {}
        Term(double scalar, std::string name) : m_scalar{scalar}, m_name{std::move(name)} {}
        explicit Term(double scalar) : m_scalar{scalar}, m_name{std::nullopt} {}

        auto scalar() -> double { return m_scalar; }
        auto name() -> std::optional<std::string> & { return m_name; }
    };

    enum class Comparison { GreaterThan, GreaterThenEquals, LessThan, LessThanEquals };

    std::vector<Term> m_left;
    Comparison m_cmp;
    std::vector<Term> m_right;

    Predicate(std::vector<Term> left, Comparison cmp, std::vector<Term> right)
        : m_left{std::move(left)}, m_cmp{cmp}, m_right{std::move(right)} {}
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

    Binary(Subtree left, Subtree right) : m_left{std::make_shared<Subtree>(std::move(left))}, m_right{std::make_shared<Subtree>(std::move(right))} {}

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

template <typename Subtree>
struct BoundedUnary : public Unary<Subtree> {
    interval::Interval m_interval;

    BoundedUnary(interval::Interval i, Subtree inner)
        : m_interval{std::move(i)}, Unary<Subtree>(std::move(inner)) {}

    BoundedUnary(interval::Interval i, std::shared_ptr<Subtree> inner)
        : m_interval{std::move(i)}, Unary<Subtree>(std::move(inner)) {}
};

template <typename Subtree>
struct BoundedGlobally : public BoundedUnary<Subtree> {
    using BoundedUnary<Subtree>::BoundedUnary;

    template <typename V>
    auto accept(V &visitor) const {
        return this->m_inner.visit_bounded_globally(this->m_interval,
                                                    this->m_inner->accept(visitor));
    }
};

template <typename Subtree>
struct BoundedFinally : public BoundedUnary<Subtree> {
    using BoundedUnary<Subtree>::BoundedUnary;

    template <typename V>
    auto accept(V &visitor) const {
        return this->m_inner.visit_bounded_finally(this->m_interval,
                                                   this->m_inner->accept(visitor));
    }
};

template <typename Subtree>
struct BoundedBinary : public Binary<Subtree> {
    interval::Interval m_interval;

    BoundedBinary(interval::Interval i, Subtree left, Subtree right)
        : m_interval{std::move(i)}, Binary<Subtree>(std::move(left), std::move(right)) {}

    BoundedBinary(interval::Interval i, std::shared_ptr<Subtree> left,
                  std::shared_ptr<Subtree> right)
        : m_interval{std::move(i)}, Binary<Subtree>(std::move(left), std::move(right)) {}
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
