#ifndef FOREK_FORMULA_CORE_OPERATION_BINARY_HPP
#define FOREK_FORMULA_CORE_OPERATION_BINARY_HPP

#include <memory>

#include <forek/formula/core/operation/operation.hpp>
#include <forek/formula/visitor/visitor.hpp>

namespace forek::formula::core::operation {
class Binary : public Operation {
   protected:
    std::unique_ptr<Node> lexpr_;
    std::unique_ptr<Node> rexpr_;

   public:
    Binary() = delete;
    auto accept(visitor::Visitor& visitor) -> void const override = 0;
};
}  // namespace forek::formula::core::operation

#endif
