#pragma once

#include <initializer_list>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace forek::algebra {
class Literal {
    double m_value;

   public:
    explicit Literal(double value) : m_value{value} {}

    template <typename V>
    auto accept(V& visitor) const {
        return visitor.visit_value(m_value);
    }
};

class Variable {
    std::string m_name;

   public:
    explicit Variable(std::string name) : m_name{std::move(name)} {}

    template <typename V>
    auto accept(V& visitor) const {
        return visitor.visit_value(m_name);
    }
};

template <typename Subtree>
struct Operator {
    std::shared_ptr<Subtree> m_lhs;
    std::shared_ptr<Subtree> m_rhs;

    Operator(std::shared_ptr<Subtree> lhs, std::shared_ptr<Subtree> rhs)
        : m_lhs{std::move(lhs)}, m_rhs{std::move(rhs)} {}

    Operator(Subtree lhs, Subtree rhs)
        : m_lhs{std::make_shared<Subtree>(std::move(lhs))},
          m_rhs{std::make_shared<Subtree>(std::move(rhs))} {}
};

template <typename Subtree>
class Add : public Operator<Subtree> {
   public:
    using Operator<Subtree>::Operator;

    template <typename V>
    auto accept(V& visitor) const {
        return visitor.visit_addition(this->m_lhs->evaluate(visitor),
                                      this->m_rhs->evaluate(visitor));
    }
};

template <typename Subtree>
class Sub : public Operator<Subtree> {
    using Operator<Subtree>::Operator;

   public:
    template <typename V>
    auto accept(V& visitor) const {
        return visitor.visit_subtraction(this->m_lhs->evaluate(visitor),
                                         this->m_rhs->evaluate(visitor));
    }
};

template <typename Subtree>
class Mult : public Operator<Subtree> {
    using Operator<Subtree>::Operator;

   public:
    template <typename V>
    auto accept(V& visitor) const {
        return visitor.visit_multiplication(this->m_lhs->evaluate(visitor),
                                            this->m_rhs->evaluate(visitor));
    }
};

template <typename Subtree>
class Div : public Operator<Subtree> {
    using Operator<Subtree>::Operator;

   public:
    template <typename V>
    auto accept(V& visitor) const {
        return visitor.visit_division(this->m_lhs->evaluate(visitor),
                                      this->m_rhs->evaluate(visitor));
    }
};

template <typename Subtree>
class Mod : public Operator<Subtree> {
    using Operator<Subtree>::Operator;

   public:
    template <typename V>
    auto accept(V& visitor) const {
        return visitor.visit_modulo(this->m_lhs->evaluate(visitor), this->m_rhs->evaluate(visitor));
    }
};

template <typename T>
class Visitor {
    virtual auto visit_value(double value) -> T = 0;
    virtual auto visit_variable(std::string name) -> T = 0;
    virtual auto visit_addition(T lhs, T rhs) -> T = 0;
    virtual auto visit_subtraction(T lhs, T rhs) -> T = 0;
    virtual auto visit_multiplication(T lhs, T rhs) -> T = 0;
    virtual auto visit_division(T lhs, T rhs) -> T = 0;
    virtual auto visit_modulo(T lhs, T rhs) -> T = 0;
};

enum class Comparison { GreaterThan, GreaterThanEqual, Equal, LessThanEqual, LessThan, NotEqual };

class Expr {
    using Node =
        std::variant<Literal, Variable, Add<Expr>, Sub<Expr>, Mult<Expr>, Div<Expr>, Mod<Expr>>;

    Node m_node;

   public:
    explicit Expr(Node node) : m_node{std::move(node)} {}

    template <typename T>
    auto evaluate(Visitor<T>& v) const -> T {
        return std::visit([&v](const auto& node) { return node.accept(v); }, m_node);
    }

    template <typename T>
    [[nodiscard]] auto is_type() const -> bool {
        return std::holds_alternative<T>(m_node);
    }

    template <template <typename> typename T>
    [[nodiscard]] auto is_type() const -> bool {
        return std::holds_alternative<T<Expr>>(m_node);
    }

    [[nodiscard]] auto is_value() const -> bool {
        return is_type<Literal>() || is_type<Variable>();
    }

    [[nodiscard]] auto is_operator() const -> bool {
        return is_type<Add>() || is_type<Sub>() || is_type<Mult>() || is_type<Div>() ||
               is_type<Mod>();
    }
};

class Sum {
    struct Term {
        double m_scalar;
        std::optional<std::string> m_name;

        Term(double scalar)  // NOLINT: google-explicit-constructors
            : m_scalar{scalar}, m_name{std::nullopt} {}

        Term(std::string name)  // NOLINT: google-explicit-constructors
            : m_scalar{1.0}, m_name{std::move(name)} {}

        Term(double scalar, std::string name) : m_scalar{scalar}, m_name{std::move(name)} {}
        Term(std::string name, double scalar) : m_scalar{scalar}, m_name{std::move(name)} {}
    };

    struct Terms {
        std::vector<std::pair<std::string, double>> coefficients;
        double constant;
    };

    double m_constant;
    std::unordered_map<std::string, double> m_coefficients;

   public:
    Sum();
    Sum(std::initializer_list<Term>);

    explicit Sum(Term term);

    auto operator+=(const Sum& lhs) -> Sum&;
    auto operator-=(const Sum& lhs) -> Sum&;
    auto operator*=(double scalar) -> Sum&;

    auto add_term(const std::string& variable, double coefficient) -> Sum&;
    auto add_term(double constant) -> Sum&;

    [[nodiscard]] auto has_coefficients() const -> bool { return m_coefficients.size() > 0; }
    [[nodiscard]] auto has_constant() const -> bool { return m_constant != 0.0; }
    [[nodiscard]] auto coefficients() const -> std::vector<std::pair<std::string, double>>;
    [[nodiscard]] auto constant() const -> double;
};

class UnsupportedOperationException : std::exception {};
auto canonical_sum(const Expr& expr) -> Sum;
}  // namespace forek::algebra
