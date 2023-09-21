#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "forek/algebra/visitor.h"

namespace forek::algebra {

class Literal {
   public:
    explicit Literal(double value);

    auto operator==(const Literal& rhs) const -> bool;

    [[nodiscard]]
    auto value() const -> double;

    template <typename T>
    auto accept(Visitor<T>& v) const -> T {
        return v.visit_value(m_value);
    }

   private:
    double m_value;
};

class Variable {
   public:
    explicit Variable(std::string name); 

    auto operator==(const Variable& rhs) const -> bool;

    [[nodiscard]]
    auto name() const -> std::string_view;

    template <typename T>
    auto accept(Visitor<T>& v) const -> T {
        return v.visit_variable(m_name);
    }

   private:
    std::string m_name;
};

template <typename Subtree>
class Operator {
   public:
    template <typename L, typename R>
    Operator(L&& lhs, R&& rhs)
        : m_lhs{std::make_shared<Subtree>(std::forward<L>(lhs))}
        , m_rhs{std::make_shared<Subtree>(std::forward<R>(rhs))} {}

    Operator(std::shared_ptr<Subtree> lhs, std::shared_ptr<Subtree> rhs)
        : m_lhs{std::move(lhs)}, m_rhs{std::move(rhs)} {}

    template<typename T>
    auto evaluate_lhs(Visitor<T>& v) const -> T {
        return m_lhs->evaluate(v);
    }

    template<typename T>
    auto evaluate_rhs(Visitor<T>& v) const -> T {
        return m_rhs->evaluate(v);
    }

    auto operator==(const Operator<Subtree>& rhs) const -> bool {
        return *m_lhs == *rhs.m_lhs && *m_rhs == *rhs.m_rhs;
    }

   private:
    std::shared_ptr<Subtree> m_lhs;
    std::shared_ptr<Subtree> m_rhs;
};

template <typename Subtree>
class Add : public Operator<Subtree> {
   public:
    using Operator<Subtree>::Operator;

    auto operator==(const Add<Subtree>& rhs) const -> bool {
        return Operator<Subtree>::operator==(rhs);
    }

    template <typename T>
    auto accept(Visitor<T>& v) const -> T {
        return v.visit_addition(this->evaluate_lhs(v), this->evaluate_rhs(v));
    }
};

template <typename Subtree>
class Sub : public Operator<Subtree> {
   public:
    using Operator<Subtree>::Operator;

    auto operator==(const Sub<Subtree>& rhs) const -> bool {
        return Operator<Subtree>::operator==(rhs);
    }

    template <typename T>
    auto accept(Visitor<T>& v) const -> T {
        return v.visit_subtraction(this->evaluate_lhs(v), this->evaluate_rhs(v));
    }
};

template <typename Subtree>
struct Mult : public Operator<Subtree> {
   public:
    using Operator<Subtree>::Operator;

    auto operator==(const Mult<Subtree>& rhs) const -> bool {
        return Operator<Subtree>::operator==(rhs);
    }

    template <typename T>
    auto accept(Visitor<T>& v) const -> T {
        return v.visit_multiplication(this->evaluate_lhs(v), this->evaluate_rhs(v));
    }
};

template <typename Subtree>
class Div : public Operator<Subtree> {
   public:
    using Operator<Subtree>::Operator;
    
    auto operator==(const Div<Subtree>& rhs) const -> bool {
        return Operator<Subtree>::operator==(rhs);
    }

    template <typename T>
    auto accept(Visitor<T>& v) const -> T {
        return v.visit_division(this->evaluate_lhs(v), this->evaluate_rhs(v));
    }
};

template <typename Subtree>
struct Mod : public Operator<Subtree> {
   public:
    using Operator<Subtree>::Operator;

    auto operator==(const Mod<Subtree>& rhs) const -> bool {
        return Operator<Subtree>::operator==(rhs);
    }

    template <typename T>
    auto accept(Visitor<T>& v) const -> T {
        return v.visit_modulo(this->evaluate_lhs(v), this->evaluate_rhs(v));
    }
};

}  //namespace forek::algebra
