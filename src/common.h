#pragma once

#include <any>
#include <limits>
#include <memory>
#include <string>

#include "forek/interval.h"

namespace forek::common {

using interval::Interval;
using interval::make_exclusive;
using interval::make_inclusive;

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
    auto lower = ctx->LeftBracket() ? make_inclusive(lval) : make_exclusive(lval);
    auto upper = ctx->RightBracket() ? make_inclusive(uval) : make_exclusive(uval);

    return Interval{lower, upper};
}
}  // namespace forek::common
