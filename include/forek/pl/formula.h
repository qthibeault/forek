#pragma once

#include <memory>
#include <string>
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

    explicit operator std::string() const;

    auto operator==(const Formula&) const -> bool;
    explicit operator std::string();

    template <typename T>
    auto evaluate(Visitor<T>& v) const -> T {
        return m_root->accept(v);
    }
};

}  // namespace forek::pl
