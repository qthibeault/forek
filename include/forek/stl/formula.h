#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "forek/stl/tree.h"
#include "forek/stl/visitor.h"

namespace forek::stl {

class Formula {
    std::shared_ptr<Tree> m_root;

   public:
    explicit Formula(std::string_view formula);
    explicit Formula(Tree tree);
    explicit Formula(std::shared_ptr<Tree> tree);

    auto operator==(const Formula& lhs) -> bool;
    explicit operator std::string();

    template <typename T>
    auto evaluate(Visitor<T>& v) const -> T {
        return m_root->accept(v);
    }
};

}  // namespace forek::stl
