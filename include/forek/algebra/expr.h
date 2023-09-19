#pragma once

#include <string>
#include <utility>
#include <variant>

#include "forek/algebra/ast.h"

namespace forek::algebra {

enum class Comparison { GreaterThan, GreaterThanEqual, Equal, LessThanEqual, LessThan, NotEqual };

class Expr {
   private:
    using Node = std::variant<Literal, Variable, Add<Expr>, Sub<Expr>, Mult<Expr>, Div<Expr>, Mod<Expr>>;

   public:
    explicit Expr(Node node);

    auto operator==(const Expr& rhs) const -> bool;
    explicit operator std::string() const;

    template <typename T>
    [[nodiscard]] auto is_type() const -> bool {
        return std::holds_alternative<T>(m_node);
    }

    template <template <typename> typename T>
    [[nodiscard]] auto is_type() const -> bool {
        return std::holds_alternative<T<Expr>>(m_node);
    }

    template <typename T>
    auto accept(Visitor<T>& v) const -> T {
        return std::visit([&v](const auto& node) { return node.accept(v); }, m_node);
    }

    [[nodiscard]] auto is_value() const -> bool;
    [[nodiscard]] auto is_operator() const -> bool;

   private:
    Node m_node;
};

}  // namespace forek::algebra
