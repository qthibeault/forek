#include <any>
#include <string>

#include "forek/ltl/ir.h"
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
        auto neg = Negation<Tree>{std::any_cast<Tree>(inner)};

        return Tree{std::move(neg)};
    }

    auto visitPlConjunction(LinearTemporalLogicParser::PlConjunctionContext *ctx)
        -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto conj = Conjunction<Tree>{std::any_cast<Tree>(left), std::any_cast<Tree>(right)};

        return Tree{std::move(conj)};
    }

    auto visitPlDisjunction(LinearTemporalLogicParser::PlDisjunctionContext *ctx)
        -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto conj = Disjunction<Tree>{std::any_cast<Tree>(left), std::any_cast<Tree>(right)};

        return Tree{std::move(conj)};
    }

    auto visitPlImplication(LinearTemporalLogicParser::PlImplicationContext *ctx)
        -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto conj = Implication<Tree>{std::any_cast<Tree>(left), std::any_cast<Tree>(right)};

        return Tree{std::move(conj)};
    }

    auto visitPlIff(LinearTemporalLogicParser::PlIffContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto conj = Equivalence<Tree>{std::any_cast<Tree>(left), std::any_cast<Tree>(right)};

        return Tree{std::move(conj)};
    }

    auto visitLtlAlways(LinearTemporalLogicParser::LtlAlwaysContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto globally = Globally<Tree>{std::any_cast<Tree>(inner)};

        return Tree{std::move(globally)};
    }

    auto visitLtlEventually(LinearTemporalLogicParser::LtlEventuallyContext *ctx)
        -> std::any override {
        auto inner = visit(ctx->formula());
        auto finally = Finally<Tree>{std::any_cast<Tree>(inner)};

        return Tree{std::move(finally)};
    }

    auto visitLtlNext(LinearTemporalLogicParser::LtlNextContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto next = Next<Tree>{std::any_cast<Tree>(inner)};

        return Tree{std::move(next)};
    }

    auto visitLtlRelease(LinearTemporalLogicParser::LtlReleaseContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto release = Release<Tree>{std::any_cast<Tree>(left), std::any_cast<Tree>(right)};

        return Tree{std::move(release)};
    }

    auto visitLtlUntil(LinearTemporalLogicParser::LtlUntilContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto release = Until<Tree>{std::any_cast<Tree>(left), std::any_cast<Tree>(right)};

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

Formula::Formula(Tree root) { this->m_root = std::make_shared<Tree>(std::move(root)); }
