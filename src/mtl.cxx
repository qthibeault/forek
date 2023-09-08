#include <any>
#include <string>

#include "forek/interval.h"
#include "forek/mtl/formula.h"
#include "forek/mtl/ir.h"
#include "forek/mtl/tree.h"

#include "MetricTemporalLogicLexer.h"
#include "MetricTemporalLogicParser.h"
#include "MetricTemporalLogicParserVisitor.h"
#include "common.h"
#include "listeners.h"

#include "antlr4-runtime.h"

using forek::MetricTemporalLogicLexer;
using forek::MetricTemporalLogicParser;
using forek::MetricTemporalLogicParserVisitor;
using forek::common::make_binary;
using forek::common::make_interval;
using forek::common::make_unary;
using forek::common::StringBuilder;
using forek::listeners::LexerErrorListener;
using forek::listeners::ParserErrorListener;
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

namespace forek::mtl {

class Builder : public MetricTemporalLogicParserVisitor {
    using Parser = MetricTemporalLogicParser;

    auto visitStart(Parser::StartContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitParentheses(Parser::ParenthesesContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitPlTrue(Parser::PlTrueContext *) -> std::any override {
        return std::make_shared<Tree>(True{});
    }

    auto visitPlFalse(Parser::PlFalseContext *) -> std::any override {
        return std::make_shared<Tree>(False{});
    }

    auto visitProposition(Parser::PropositionContext *ctx) -> std::any override {
        return std::make_shared<Tree>(Proposition{ctx->Identifier()->getText()});
    }

    auto visitPlProposition(Parser::PlPropositionContext *ctx) -> std::any override {
        return visit(ctx->proposition());
    }

    auto visitPlNegation(Parser::PlNegationContext *ctx) -> std::any override {
        return make_unary<Negation, Tree>(visit(ctx->formula()));
    }

    auto visitPlConjunction(Parser::PlConjunctionContext *ctx) -> std::any override {
        return make_binary<Conjunction, Tree>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }

    auto visitPlDisjunction(Parser::PlDisjunctionContext *ctx) -> std::any override {
        return make_binary<Disjunction, Tree>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }

    auto visitPlImplication(Parser::PlImplicationContext *ctx) -> std::any override {
        return make_binary<Implication, Tree>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }

    auto visitPlIff(Parser::PlIffContext *ctx) -> std::any override {
        return make_binary<Equivalence, Tree>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }

    auto visitLtlAlways(Parser::LtlAlwaysContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto interval = ctx->interval();

        if (interval) {
            return make_unary<BoundedGlobally, Tree>(visit(interval), inner);
        }

        return make_unary<Globally, Tree>(inner);
    }

    auto visitLtlEventually(Parser::LtlEventuallyContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto interval = ctx->interval();

        if (interval) {
            return make_unary<BoundedFinally, Tree>(visit(interval), inner);
        }

        return make_unary<Finally, Tree>(inner);
    }

    auto visitLtlNext(Parser::LtlNextContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto interval = ctx->interval();

        if (interval) {
            return make_unary<BoundedNext, Tree>(visit(interval), inner);
        }

        return make_unary<Next, Tree>(inner);
    }

    auto visitLtlRelease(Parser::LtlReleaseContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto interval = ctx->interval();

        if (interval) {
            return make_binary<BoundedRelease, Tree>(visit(interval), left, right);
        }

        return make_binary<Release, Tree>(left, right);
    }

    auto visitLtlUntil(Parser::LtlUntilContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto interval = ctx->interval();

        if (interval) {
            return make_binary<BoundedUntil, Tree>(visit(interval), left, right);
        }

        return make_binary<Until, Tree>(left, right);
    }

    auto visitInterval(Parser::IntervalContext *ctx) -> std::any override {
        return make_interval(ctx);
    }
};

auto parse_formula(std::string_view formula) -> std::shared_ptr<Tree> {
    auto input_stream = antlr4::ANTLRInputStream(formula);
    auto lexer = MetricTemporalLogicLexer(&input_stream);
    auto lexer_listener = std::make_unique<LexerErrorListener>();

    lexer.removeErrorListeners();
    lexer.addErrorListener(lexer_listener.get());

    auto token_stream = antlr4::CommonTokenStream(&lexer);
    auto parser = MetricTemporalLogicParser(&token_stream);
    auto parser_listener = std::make_unique<ParserErrorListener>();

    parser.removeErrorListeners();
    parser.addErrorListener(parser_listener.get());

    auto builder = Builder{};
    auto output = builder.visit(parser.start());

    return std::any_cast<std::shared_ptr<Tree>>(output);
}

Formula::Formula(std::string_view formula) : m_root{parse_formula(std::move(formula))} {}
Formula::Formula(Tree root) : m_root{std::make_shared<Tree>(std::move(root))} {}

Formula::operator std::string() const {
    StringBuilder sb;
    return this->evaluate(sb);
}

auto Formula::operator==(const Formula& lhs) const -> bool {
    return *this->m_root == *lhs.m_root;
}

auto Tree::operator==(const Tree& lhs) const -> bool {
    return this->m_inner == lhs.m_inner;
}

}  // namespace forek::mtl

