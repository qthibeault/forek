#include <any>

#include "forek/ltl/formula.h"
#include "forek/ltl/ir.h"
#include "forek/ltl/tree.h"

#include "LinearTemporalLogicLexer.h"
#include "LinearTemporalLogicParser.h"
#include "LinearTemporalLogicParserVisitor.h"
#include "common.h"
#include "listeners.h"

#include "antlr4-runtime.h"

using forek::LinearTemporalLogicLexer;
using forek::LinearTemporalLogicParser;
using forek::LinearTemporalLogicParserVisitor;
using forek::common::make_binary;
using forek::common::make_unary;
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
using forek::listeners::LexerErrorListener;
using forek::listeners::ParserErrorListener;
using forek::ltl::Formula;
using forek::ltl::Tree;

class FormulaBuilder : public LinearTemporalLogicParserVisitor {
    using Parser = LinearTemporalLogicParser;

    auto visitStart(Parser::StartContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitParentheses(Parser::ParenthesesContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitPlTrue(Parser::PlTrueContext *ctx) -> std::any override {
        return std::make_shared<Tree>(True{});
    }

    auto visitPlFalse(Parser::PlFalseContext *ctx) -> std::any override {
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
        return make_unary<Globally, Tree>(visit(ctx->formula()));
    }

    auto visitLtlEventually(Parser::LtlEventuallyContext *ctx) -> std::any override {
        return make_unary<Finally, Tree>(visit(ctx->formula()));
    }

    auto visitLtlNext(Parser::LtlNextContext *ctx) -> std::any override {
        return make_unary<Next, Tree>(visit(ctx->formula()));
    }

    auto visitLtlRelease(Parser::LtlReleaseContext *ctx) -> std::any override {
        return make_binary<Release, Tree>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }

    auto visitLtlUntil(Parser::LtlUntilContext *ctx) -> std::any override {
        return make_binary<Until, Tree>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }
};

auto parse_formula(std::string_view formula) -> std::shared_ptr<Tree> {
    auto input_stream = antlr4::ANTLRInputStream(formula);
    auto lexer = LinearTemporalLogicLexer(&input_stream);
    auto lexer_listener = std::make_unique<LexerErrorListener>();

    lexer.removeErrorListeners();
    lexer.addErrorListener(lexer_listener.get());

    auto token_stream = antlr4::CommonTokenStream(&lexer);
    auto parser = LinearTemporalLogicParser(&token_stream);
    auto parser_listener = std::make_unique<ParserErrorListener>();

    parser.removeErrorListeners();
    parser.addErrorListener(parser_listener.get());

    auto builder = FormulaBuilder{};
    auto output = builder.visit(parser.start());

    return std::any_cast<std::shared_ptr<Tree>>(output);
}

Formula::Formula(std::string_view formula) : m_root{parse_formula(formula)} {}
Formula::Formula(Tree root) : m_root{std::make_shared<Tree>(std::move(root))} {}
