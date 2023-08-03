#pragma once

#include "forek/pl/visitor.h"
#include "forek/pl/tree.h"

namespace forek::pl {
class Formula {
   public:
    Formula() = delete;
    Formula(const Formula& other) = default;
    Formula(Formula&& other) = default;
    ~Formula() = default;

    explicit Formula(std::string formula);

    template <typename T>
    auto evaluate(forek::pl::Visitor<T>& v) -> T {
        return m_root->visit(v);
    }

   private:
    std::shared_ptr<Tree> m_root;
};
}  // namespace forek::pl
