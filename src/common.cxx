#include "fmt/format.h"

#include "common.h"

namespace forek::common {

    class ExprBuilder {};
    
    auto StringBuilder::visit_true() -> std::string { return "True"; }
    auto StringBuilder::visit_false() -> std::string { return "False"; }
    auto StringBuilder::visit_proposition(std::string name) -> std::string { return name; }

    auto visit_predicate(const Expr& lhs, Comparison cmp, const Expr& rhs) -> std::string {
        return "";
    }

    auto StringBuilder::visit_negation(std::string inner) -> std::string {
        return fmt::format("not ({})", inner);
    }

    auto StringBuilder::visit_conjunction(std::string left, std::string right) -> std::string {
        return fmt::format("({}) and ({})", left, right);
    }

    auto StringBuilder::visit_disjunction(std::string left, std::string right) -> std::string {
        return fmt::format("({}) or ({})", left, right);
    }

    auto StringBuilder::visit_implication(std::string ante, std::string cons) -> std::string {
        return fmt::format("({}) implies ({})", ante, cons);
    }

    auto StringBuilder::visit_equivalence(std::string ante, std::string cons) -> std::string {
        return fmt::format("({}) if ({})", ante, cons);
    }

    auto StringBuilder::visit_next(std::string inner) -> std::string {
        return fmt::format("next ({})", inner);
    }

    auto StringBuilder::visit_globally(std::string inner) -> std::string {
        return fmt::format("always ({})", inner);
    }

    auto StringBuilder::visit_finally(std::string inner) -> std::string {
        return fmt::format("eventually ({})", inner);
    }

    auto StringBuilder::visit_until(std::string left, std::string right) -> std::string {
        return fmt::format("({}) until ({})", left, right);
    }

    auto StringBuilder::visit_release(std::string left, std::string right) -> std::string {
        return fmt::format("({}) release ({})", left, right);
    }

    auto StringBuilder::visit_bounded_next(const Interval& i, std::string inner) -> std::string {
        return fmt::format("next{} ({})", i, inner);
    }

    auto StringBuilder::visit_bounded_globally(const Interval& i, std::string inner) -> std::string {
        return fmt::format("always{} ({})", i, inner);
    }

    auto StringBuilder::visit_bounded_finally(const Interval& i, std::string inner) -> std::string {
        return fmt::format("eventually{} ({})", i, inner);
    }

    auto StringBuilder::visit_bounded_until(const Interval& i, std::string left, std::string right) -> std::string {
        return fmt::format("({}) until{} ({})", left, i, right);
    }

    auto StringBuilder::visit_bounded_release(const Interval& i, std::string left, std::string right) -> std::string {
        return fmt::format("({}) release{} ({})", left, i, right);
    }

}  // namespace forek::common
