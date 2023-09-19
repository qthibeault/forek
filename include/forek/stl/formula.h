#pragma once

#include <memory>
#include <ostream>
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

    auto operator==(const Formula& lhs) const -> bool;
    explicit operator std::string() const;

    template <typename T>
    auto evaluate(Visitor<T>& v) const -> T {
        return m_root->accept(v);
    }
};

auto operator<<(std::ostream& os, const Formula& f) -> std::ostream&;

}  // namespace forek::stl
