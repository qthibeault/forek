#pragma once

#include <ostream>
#include <memory>
#include <string>
#include <string_view>

#include "forek/ltl/tree.h"
#include "forek/ltl/visitor.h"

namespace forek::ltl {

class Formula {
   public:
    explicit Formula(std::string_view formula);
    explicit Formula(Tree root);

    auto operator==(const Formula& lhs) const -> bool;
    explicit operator std::string() const;

    template <typename T>
    auto evaluate(Visitor<T>& v) const -> T {
        return m_root->accept(v);
    }

   private:
    std::shared_ptr<Tree> m_root;
};

auto operator<<(std::ostream& os, const Formula& f) -> std::ostream&;

class PastTimeFormula {
   public:
    explicit PastTimeFormula(std::string formula);
};

}  // namespace forek::ltl
