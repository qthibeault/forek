#include <any>
#include <string>

#include "forek/ir.h"
#include "forek/ltl/formula.h"
#include "forek/ltl/tree.h"

#include "LinearTemporalLogicLexer.h"
#include "LinearTemporalLogicParser.h"
#include "LinearTemporalLogicParserVisitor.h"

#include "antlr4-runtime.h"

using forek::LinearTemporalLogicLexer;
using forek::LinearTemporalLogicParser;
using forek::LinearTemporalLogicParserVisitor;
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
using forek::ltl::Formula;
using forek::ltl::Tree;

class FormulaBuilder : public LinearTemporalLogicParserVisitor {
    auto visitStart(LinearTemporalLogicParser::StartContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitParentheses(LinearTemporalLogicParser::ParenthesesContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitPlTrue(LinearTemporalLogicParser::PlTrueContext *ctx) -> std::any override {
        return Tree{True{}};
    }

    auto visitPlFalse(LinearTemporalLogicParser::PlFalseContext *ctx) -> std::any override {
        return Tree{False{}};
    }

    auto visitProposition(LinearTemporalLogicParser::PropositionContext *ctx) -> std::any override {
        auto name = ctx->Identifier()->getText();
        auto prop = Proposition{std::move(name)};

        return Tree{std::move(prop)};
    }

    auto visitPlProposition(LinearTemporalLogicParser::PlPropositionContext *ctx)
        -> std::any override {
        return visit(ctx->proposition());
    }

    auto visitPlNegation(LinearTemporalLogicParser::PlNegationContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto subtree = std::any_cast<Tree>(inner);
        auto neg = Negation{std::move(subtree)};

        return Tree{std::move(neg)};
    }

    auto visitPlConjunction(LinearTemporalLogicParser::PlConjunctionContext *ctx)
        -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto conj = Conjunction{std::move(left_subtree), std::move(right_subtree)};

        return Tree{std::move(conj)};
    }

    auto visitPlDisjunction(LinearTemporalLogicParser::PlDisjunctionContext *ctx)
        -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto conj = Disjunction{std::move(left_subtree), std::move(right_subtree)};

        return Tree{std::move(conj)};
    }

    auto visitPlImplication(LinearTemporalLogicParser::PlImplicationContext *ctx)
        -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto conj = Implication{std::move(left_subtree), std::move(right_subtree)};

        return Tree{std::move(conj)};
    }

    auto visitPlIff(LinearTemporalLogicParser::PlIffContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto conj = Equivalence{std::move(left_subtree), std::move(right_subtree)};

        return Tree{std::move(conj)};
    }

    auto visitLtlAlways(LinearTemporalLogicParser::LtlAlwaysContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto subtree = std::any_cast<Tree>(inner);
        auto globally = Globally{std::move(subtree)};

        return Tree{std::move(globally)};
    }

    auto visitLtlEventually(LinearTemporalLogicParser::LtlEventuallyContext *ctx)
        -> std::any override {
        auto inner = visit(ctx->formula());
        auto subtree = std::any_cast<Tree>(inner);
        auto finally = Finally{std::move(subtree)};

        return Tree{std::move(finally)};
    }

    auto visitLtlNext(LinearTemporalLogicParser::LtlNextContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto subtree = std::any_cast<Tree>(inner);
        auto next = Next{std::move(subtree)};

        return Tree{std::move(next)};
    }

    auto visitLtlRelease(LinearTemporalLogicParser::LtlReleaseContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto release = Release{std::move(left_subtree), std::move(right_subtree)};

        return Tree{std::move(release)};
    }

    auto visitLtlUntil(LinearTemporalLogicParser::LtlUntilContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto release = Until{std::move(left_subtree), std::move(right_subtree)};

        return Tree{std::move(release)};
    }
};

Formula::Formula(std::string formula) {
    auto input_stream = antlr4::ANTLRInputStream(formula);
    auto lexer = LinearTemporalLogicLexer(&input_stream);
    auto token_stream = antlr4::CommonTokenStream(&lexer);
    auto parser = LinearTemporalLogicParser(&token_stream);
    auto builder = FormulaBuilder{};
    auto output = builder.visit(parser.start());
    auto root = std::any_cast<Tree>(output);

    this->m_root = std::make_shared<Tree>(std::move(root));
}
