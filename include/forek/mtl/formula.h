#pragma once

#include <memory>
#include <string>

#include "forek/mtl/tree.h"
#include "forek/mtl/visitor.h"

namespace forek::mtl {
class Formula {
    std::shared_ptr<Tree> m_root;

   public:
    explicit Formula(std::string formula);
    explicit Formula(Tree root);

    template <typename T>
    auto evaluate(Visitor<T>& v) const -> T {
        return m_root->accept(v);
    }
};
}  // namespace forek::mtl
