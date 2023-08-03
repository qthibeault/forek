#pragma once

#include <string>

#include "forek/ltl/tree.h"
#include "forek/ltl/visitor.h"

namespace forek::ltl {
class Formula {
   private:
    std::shared_ptr<Tree> m_root;

   public:
    Formula() = delete;
    Formula(const Formula& other) = default;
    Formula(Formula&& other) = default;

    explicit Formula(std::string formula);

    template <typename T>
    auto evaluate(Visitor<T>& v) {
        return m_root->visit(v);
    }
};

class PastTimeFormula {
   public:
    explicit PastTimeFormula(std::string formula);
};
}  // namespace forek::ltl
