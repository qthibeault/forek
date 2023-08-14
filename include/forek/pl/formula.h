#pragma once

#include "forek/pl/tree.h"
#include "forek/pl/visitor.h"

namespace forek::pl {
class Formula {
   private:
    std::shared_ptr<Tree> m_root;

   public:
    Formula() = delete;
    Formula(const Formula& other) = default;
    Formula(Formula&& other) = default;
    ~Formula() = default;

    explicit Formula(std::string formula);
    explicit Formula(Tree root);

    template <typename T>
    auto evaluate(forek::pl::Visitor<T>& v) -> T {
        return m_root->visit(v);
    }
};
}  // namespace forek::pl
