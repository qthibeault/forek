#pragma once

#include <string>

namespace forek::ltl {
class Formula {
   public:
    explicit Formula(std::string formula);
};

class PastTimeFormula {
   public:
    explicit PastTimeFormula(std::string formula);
};
}  // namespace forek::ltl
