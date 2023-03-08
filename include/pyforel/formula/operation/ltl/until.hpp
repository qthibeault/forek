#ifndef PYFOREL_FORMULA_OPERATION_LTL_UNTIL_HPP
#define PYFOREL_FORMULA_OPERATION_LTL_UNTIL_HPP

#include <memory>

#include <forek/formula/core/node.hpp>
#include <forek/formula/operation/binary.hpp>

namespace forek::formula::operation::ltl {
namespace kind {
enum { Until = 10 };
}

class Until : public Binary {
   public:
    Until() = delete;
    Until(std::unique_ptr<core::Node> lop, std::unique_ptr<core::Node> rop)
        : Binary(kind::Until, std::move(lop), std::move(rop)) {}

    [[nodiscard]] inline auto clone() const -> std::unique_ptr<Node> override {
        return std::make_unique<Until>(std::move(this->lop()->clone()),
                                       std::move(this->rop()->clone()));
    }
};
}  // namespace forek::formula::operation::ltl
#endif
