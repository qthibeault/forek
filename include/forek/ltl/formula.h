#pragma once

#include <string_view>

#include "forek/ltl/tree.h"
#include "forek/ltl/visitor.h"

namespace forek::ltl {
class Formula {
   private:
    std::shared_ptr<Tree> m_root;

   public:
    explicit Formula(std::string_view formula);
    explicit Formula(Tree root);

    template <typename T>
    auto evaluate(Visitor<T> &v) {
        return m_root->accept(v);
    }
};

class PastTimeFormula {
   public:
    explicit PastTimeFormula(std::string formula);
};
}  // namespace forek::ltl
