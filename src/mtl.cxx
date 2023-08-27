#include <any>
#include <string>

#include "forek/interval.h"
#include "forek/mtl/formula.h"
#include "forek/mtl/ir.h"
#include "forek/mtl/tree.h"

#include "MetricTemporalLogicLexer.h"
#include "MetricTemporalLogicParser.h"
#include "MetricTemporalLogicParserVisitor.h"

#include "antlr4-runtime.h"

using forek::MetricTemporalLogicLexer;
using forek::MetricTemporalLogicParser;
using forek::MetricTemporalLogicParserVisitor;
using forek::interval::Endpoint;
using forek::interval::Exclusive;
using forek::interval::Inclusive;
using forek::interval::Interval;
using forek::ir::BoundedFinally;
using forek::ir::BoundedGlobally;
using forek::ir::BoundedNext;
using forek::ir::BoundedRelease;
using forek::ir::BoundedUntil;
using forek::ir::Conjunction;
using forek::ir::Disjunction;
using forek::ir::Equivalence;
using forek::ir::False;
using forek::ir::Finally;
using forek::ir::Globally;
using forek::ir::Implication;
using forek::ir::Negation;
using forek::ir::Next;
using forek::ir::Proposition;
using forek::ir::Release;
using forek::ir::True;
using forek::ir::Until;
using forek::mtl::Formula;
using forek::mtl::Tree;

using TreePtr = std::shared_ptr<Tree>;

template <template <typename> typename T>
inline auto make_unary(std::any inner) -> TreePtr {
    return std::make_shared<Tree>(T<Tree>{std::any_cast<TreePtr>(inner)});
}

template <template <typename> typename T>
inline auto make_unary(std::any interval, std::any inner) -> TreePtr {
    auto interval_ = std::any_cast<Interval>(interval);
    auto inner_ = std::any_cast<TreePtr>(inner);

    return std::make_shared<Tree>(T<Tree>{std::move(interval_), std::move(inner_)});
}

template <template <typename> typename T>
inline auto make_binary(std::any left, std::any right) -> TreePtr {
    auto left_ = std::any_cast<TreePtr>(left);
    auto right_ = std::any_cast<TreePtr>(right);

    return std::make_shared<Tree>(T<Tree>{std::move(left_), std::move(right_)});
}

template <template <typename> typename T>
inline auto make_binary(std::any interval, std::any left, std::any right) -> TreePtr {
    auto interval_ = std::any_cast<Interval>(interval);
    auto left_ = std::any_cast<TreePtr>(left);
    auto right_ = std::any_cast<TreePtr>(right);

    return std::make_shared<Tree>(
        T<Tree>{std::move(interval_), std::move(left_), std::move(right_)});
}

class Builder : public MetricTemporalLogicParserVisitor {
    using MtlParser = MetricTemporalLogicParser;

    auto visitStart(MtlParser::StartContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitParentheses(MtlParser::ParenthesesContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitPlTrue(MtlParser::PlTrueContext *ctx) -> std::any override {
        return std::make_shared<Tree>(True{});
    }

    auto visitPlFalse(MtlParser::PlFalseContext *ctx) -> std::any override {
        return std::make_shared<Tree>(False{});
    }

    auto visitProposition(MtlParser::PropositionContext *ctx) -> std::any override {
        return std::make_shared<Tree>(Proposition{ctx->Identifier()->getText()});
    }

    auto visitPlProposition(MtlParser::PlPropositionContext *ctx) -> std::any override {
        return visit(ctx->proposition());
    }

    auto visitPlNegation(MtlParser::PlNegationContext *ctx) -> std::any override {
        return make_unary<Negation>(visit(ctx->formula()));
    }

    auto visitPlConjunction(MtlParser::PlConjunctionContext *ctx) -> std::any override {
        return make_binary<Conjunction>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }

    auto visitPlDisjunction(MtlParser::PlDisjunctionContext *ctx) -> std::any override {
        return make_binary<Disjunction>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }

    auto visitPlImplication(MtlParser::PlImplicationContext *ctx) -> std::any override {
        return make_binary<Implication>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }

    auto visitPlIff(MtlParser::PlIffContext *ctx) -> std::any override {
        return make_binary<Equivalence>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }

    auto visitLtlAlways(MtlParser::LtlAlwaysContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto interval = ctx->interval();

        if (interval) {
            return make_unary<BoundedGlobally>(interval, inner);
        }

        return make_unary<Globally>(inner);
    }

    auto visitLtlEventually(MtlParser::LtlEventuallyContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto interval = ctx->interval();

        if (interval) {
            return make_unary<BoundedFinally>(interval, inner);
        }

        return make_unary<Finally>(inner);
    }

    auto visitLtlNext(MtlParser::LtlNextContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto p_interval = ctx->interval();

        if (p_interval) {
            auto interval = visit(p_interval);

            return std::make_shared<Tree>(BoundedNext<Tree>{std::any_cast<Interval>(interval),
                                                            std::any_cast<TreePtr>(inner)});
        }

        return std::make_shared<Tree>(Next<Tree>{std::any_cast<TreePtr>(inner)});
    }

    auto visitLtlRelease(MtlParser::LtlReleaseContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto interval = ctx->interval();

        if (interval) {
            return make_binary<BoundedRelease>(interval, left, right);
        }

        return make_binary<Release>(left, right);
    }

    auto visitLtlUntil(MtlParser::LtlUntilContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto interval = ctx->interval();

        if (interval) {
            return make_binary<BoundedUntil>(interval, left, right);
        }

        return make_binary<Until>(left, right);
    }

    auto visitInterval(MtlParser::IntervalContext *ctx) -> std::any override {
        auto lower_value = ctx->Infinity(0) ? std::numeric_limits<double>::infinity()
                                            : std::stod(ctx->Scalar(0)->getText());

        auto upper_value = ctx->Infinity(1) ? std::numeric_limits<double>::infinity()
                                            : std::stod(ctx->Scalar(1)->getText());

        auto lower = ctx->LeftBracket() ? Endpoint{Inclusive{lower_value}}
                                        : Endpoint{Exclusive{lower_value}};

        auto upper = ctx->RightBracket() ? Endpoint{Inclusive{upper_value}}
                                         : Endpoint{Exclusive{upper_value}};

        return Interval{lower, upper};
    }
};

Formula::Formula(std::string formula) {
    auto input_stream = antlr4::ANTLRInputStream(formula);
    auto lexer = MetricTemporalLogicLexer(&input_stream);
    auto token_stream = antlr4::CommonTokenStream(&lexer);
    auto parser = MetricTemporalLogicParser(&token_stream);
    auto builder = Builder{};
    auto output = builder.visit(parser.start());

    this->m_root = std::any_cast<TreePtr>(output);
}

Formula::Formula(Tree root) { this->m_root = std::make_shared<Tree>(std::move(root)); }
