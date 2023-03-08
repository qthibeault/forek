#ifndef PYFOREL_SPECIFICATION_LTL_LTL_HPP
#define PYFOREL_SPECIFICATION_LTL_LTL_HPP

#include <string>

#include <forek/formula/formula.hpp>
#include <forek/specification/specification.hpp>

namespace forek::specification::ltl {
class LinearTemporalLogic : public Specification {
   private:
    std::string formula_;

   public:
    LinearTemporalLogic() = delete;
    explicit LinearTemporalLogic(std::string formula)
        : formula_(std::move(formula)) {}

    /// Parse a LTL formulae into a Formula.
    [[nodiscard]] auto parse() const -> formula::Formula override;
};
}  // namespace forek::specification::ltl

#endif
