#pragma once

#include <any>
#include <limits>
#include <memory>
#include <string>

#include "forek/interval.h"
#include "forek/stl/visitor.h"

namespace forek::common {

using interval::Interval;
using interval::Endpoint;

template <template <typename> typename Op, typename Tree>
auto make_unary(std::any inner) -> std::shared_ptr<Tree> {
    return std::make_shared<Tree>(Op<Tree>{std::any_cast<std::shared_ptr<Tree>>(inner)});
}

template <template <typename> typename Op, typename Tree>
auto make_unary(std::any interval, std::any inner) -> std::shared_ptr<Tree> {
    return std::make_shared<Tree>(
        Op<Tree>{std::any_cast<Interval>(interval), std::any_cast<std::shared_ptr<Tree>>(inner)});
}

template <template <typename> typename Op, typename Tree>
auto make_binary(std::any left, std::any right) -> std::shared_ptr<Tree> {
    return std::make_shared<Tree>(Op<Tree>{std::any_cast<std::shared_ptr<Tree>>(left),
                                           std::any_cast<std::shared_ptr<Tree>>(right)});
}

template <template <typename> typename Op, typename Tree>
auto make_binary(std::any interval, std::any left, std::any right) -> std::shared_ptr<Tree> {
    return std::make_shared<Tree>(Op<Tree>{std::any_cast<Interval>(interval),
                                           std::any_cast<std::shared_ptr<Tree>>(left),
                                           std::any_cast<std::shared_ptr<Tree>>(right)});
}

template <typename T, size_t N>
auto parse_interval_value(T* ctx) -> double {
    return ctx->Infinity(N) ? std::numeric_limits<double>::infinity()
                            : std::stod(ctx->Scalar(N)->getText());
}

template <typename T>
auto make_interval(T* ctx) -> Interval {
    auto lval = parse_interval_value<T, 0>(ctx);
    auto uval = parse_interval_value<T, 1>(ctx);
    auto lower = ctx->LeftBracket() ? Endpoint::closed(lval) : Endpoint::open(lval);
    auto upper = ctx->RightBracket() ? Endpoint::closed(uval) : Endpoint::open(uval);

    return { lower, upper };
}

class StringBuilder : public stl::Visitor<std::string> {
   public:
    auto visit_true() -> std::string override;
    auto visit_false() -> std::string override;
    auto visit_proposition(std::string name) -> std::string override;
    auto visit_predicate(const Expr& lhs, Comparison cmp, const Expr& rhs) -> std::string override;
    auto visit_negation(std::string inner) -> std::string override;
    auto visit_conjunction(std::string left, std::string right) -> std::string override;
    auto visit_disjunction(std::string left, std::string right) -> std::string override;
    auto visit_implication(std::string ante, std::string cons) -> std::string override;
    auto visit_equivalence(std::string ante, std::string cons) -> std::string override;
    auto visit_next(std::string inner) -> std::string override;
    auto visit_globally(std::string inner) -> std::string override;
    auto visit_finally(std::string inner) -> std::string override;
    auto visit_until(std::string left, std::string right) -> std::string override;
    auto visit_release(std::string left, std::string right) -> std::string override;
    auto visit_bounded_next(const Interval& i, std::string inner) -> std::string override;
    auto visit_bounded_globally(const Interval& i, std::string inner) -> std::string override;
    auto visit_bounded_finally(const Interval& i, std::string inner) -> std::string override;
    auto visit_bounded_until(const Interval& i, std::string left, std::string right) -> std::string override;
    auto visit_bounded_release(const Interval& i, std::string left, std::string right) -> std::string override;
};

}  // namespace forek::common
