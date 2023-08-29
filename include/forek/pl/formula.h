#pragma once

#include <string_view>

#include "forek/pl/tree.h"
#include "forek/pl/visitor.h"

namespace forek::pl {
class Formula {
   private:
    std::shared_ptr<Tree> m_root;

   public:
    explicit Formula(std::string_view formula);
    explicit Formula(Tree root);

    template <typename T>
    auto evaluate(forek::pl::Visitor<T> &v) -> T {
        return m_root->accept(v);
    }
};
}  // namespace forek::pl
