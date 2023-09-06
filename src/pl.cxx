#include <string>

#include "forek/errors.h"
#include "forek/pl/formula.h"
#include "forek/pl/ir.h"
#include "forek/pl/tree.h"

#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "PropositionalLogicLexer.h"
#include "PropositionalLogicParser.h"
#include "PropositionalLogicParserVisitor.h"
#include "common.h"
#include "fmt/format.h"
#include "listeners.h"

using forek::PropositionalLogicLexer;
using forek::PropositionalLogicParser;
using forek::PropositionalLogicParserVisitor;
using forek::common::make_binary;
using forek::common::make_unary;
using forek::common::StringBuilder;
using forek::ir::Conjunction;
using forek::ir::Disjunction;
using forek::ir::Equivalence;
using forek::ir::False;
using forek::ir::Implication;
using forek::ir::Negation;
using forek::ir::Proposition;
using forek::ir::True;
using forek::listeners::LexerErrorListener;
using forek::listeners::ParserErrorListener;

namespace forek::pl {

class Builder : public PropositionalLogicParserVisitor {
    using Parser = PropositionalLogicParser;

   public:
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

    auto visitPlIff(PropositionalLogicParser::PlIffContext *ctx) -> std::any override {
        return make_binary<Equivalence, Tree>(visit(ctx->formula(0)), visit(ctx->formula(1)));
    }
};

auto parse_formula(std::string_view formula) -> std::shared_ptr<Tree> {
    auto input_stream = antlr4::ANTLRInputStream(formula);
    auto lexer = PropositionalLogicLexer(&input_stream);
    auto lexer_listener = std::make_unique<LexerErrorListener>();

    lexer.removeErrorListeners();
    lexer.addErrorListener(lexer_listener.get());

    auto token_stream = antlr4::CommonTokenStream(&lexer);
    auto parser = PropositionalLogicParser(&token_stream);
    auto parser_listener = std::make_unique<ParserErrorListener>();

    parser.removeErrorListeners();
    parser.addErrorListener(parser_listener.get());

    auto builder = Builder{};
    auto output = builder.visit(parser.start());

    return std::any_cast<std::shared_ptr<Tree>>(output);
}

Formula::Formula(std::string_view formula) : m_root{parse_formula(formula)} {}
Formula::Formula(Tree root) : m_root{std::make_shared<Tree>(std::move(root))} {}

Formula::operator std::string() const {
    StringBuilder builder;
    return this->evaluate(builder);
}

auto Formula::operator==(const Formula &tree) const -> bool { return *m_root == *tree.m_root; }
auto Tree::operator==(const Tree &tree) const -> bool { return m_node == tree.m_node; }

}  // namespace forek::pl
