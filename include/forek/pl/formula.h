#pragma once

#include <memory>
#include <ostream>
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

    auto operator==(const Formula&) const -> bool;
    explicit operator std::string() const;

    template <typename T>
    auto evaluate(Visitor<T>& v) const -> T {
        return m_root->accept(v);
    }
};

auto operator<<(std::ostream& os, const Formula& f) -> std::ostream&;

}  // namespace forek::pl
