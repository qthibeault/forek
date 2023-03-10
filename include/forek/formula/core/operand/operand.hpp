#ifndef FOREK_FORMULA_CORE_OPERAND_OPERAND_HPP
#define FOREK_FORMULA_CORE_OPERAND_OPERAND_HPP

#include <memory>

#include <forek/formula/core/node.hpp>
#include <forek/formula/visitor/visitor.hpp>

namespace forek::formula::core::operand {
class Operand : public Node {
   public:
    auto accept(visitor::Visitor& visitor) const -> void override = 0;

    [[nodiscard]] inline auto clone() const
        -> std::unique_ptr<core::Node> override = 0;
};
}  // namespace forek::formula::core::operand

#endif