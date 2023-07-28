#pragma once

#include <memory>
#include <string>

#include "forek/visitors.h"
#include "forek/ir.h"

namespace forek {
namespace pl {
class Formula {
   public:
    explicit Formula(std::string formula);

    template <typename T>
    auto evaluate(const forek::pl::Visitor<T>& v) -> T;

   private:
    std::unique_ptr<IR::pl::Node> root_;
};
}  // namespace pl

namespace ltl {
class Formula {
   public:
    explicit Formula(std::string formula);
};

class PastTimeFormula {
   public:
    explicit PastTimeFormula(std::string formula);
};
}  // namespace ltl

namespace mtl {
class Formula {
   public:
    explicit Formula(std::string formula);
};
}  // namespace mtl

namespace stl {

class Formula {
   public:
    explicit Formula(std::string formula);
};
}  // namespace stl

namespace tptl {

class Formula {
   public:
    explicit Formula(std::string formula);
};
}  // namespace tptl

namespace tqtl {

class Formula {
   public:
    explicit Formula(std::string formula);
};
}  // namespace tqtl

namespace stpl {

class Formula {
   public:
    explicit Formula(std::string formula);
};
}  // namespace stpl
}  // namespace forek
