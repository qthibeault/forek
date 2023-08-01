#pragma once

#include "forek/pl/visitor.h"
#include "forek/pl/tree.h"


namespace forek::pl {
class Formula {
   public:
    explicit Formula(std::string formula);

    template <typename T>
    auto evaluate(const forek::pl::Visitor<T>& v) -> T;

   private:
    std::unique_ptr<Tree> m_root;
};
}  // namespace forek::pl
