#include <string>

#include "forek/ir.h"
#include "forek/pl/tree.h"
#include "forek/pl/formula.h"
#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "PropositionalLogicLexer.h"
#include "PropositionalLogicParser.h"
#include "PropositionalLogicParserVisitor.h"

using forek::pl::Formula;
using forek::ir::True;
using forek::ir::False;
using forek::ir::Proposition;
using forek::ir::Negation;
using forek::ir::Conjunction;
using forek::ir::Disjunction;
using forek::ir::Implication;
using forek::ir::Equivalence;
using forek::pl::Tree;
using forek::PropositionalLogicLexer;
using forek::PropositionalLogicParser;
using forek::PropositionalLogicParserVisitor;

class FormulaBuilder : public PropositionalLogicParserVisitor {
    auto visitStart(PropositionalLogicParser::StartContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitParentheses(PropositionalLogicParser::ParenthesesContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitPlTrue(PropositionalLogicParser::PlTrueContext *ctx) -> std::any override {
        return Tree {True {}};
    }

    auto visitPlFalse(PropositionalLogicParser::PlFalseContext *ctx) -> std::any override {
        return Tree {False {}};
    }
    
    auto visitProposition(PropositionalLogicParser::PropositionContext *ctx) -> std::any override {
        auto name = ctx->Identifier()->getText();
        auto prop = Proposition {std::move(name)};

        return Tree {std::move(prop)};
    }

    auto visitPlProposition(PropositionalLogicParser::PlPropositionContext *ctx) -> std::any override {
        return visit(ctx->proposition());
    }

    auto visitPlNegation(PropositionalLogicParser::PlNegationContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto subtree = std::any_cast<Tree>(inner);
        auto neg = Negation {std::move(subtree)};

        return Tree {std::move(neg)};
    }

    auto visitPlConjunction(PropositionalLogicParser::PlConjunctionContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto conj = Conjunction {std::move(left_subtree), std::move(right_subtree)};
        
        return Tree {std::move(conj)};
    }

    auto visitPlDisjunction(PropositionalLogicParser::PlDisjunctionContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto conj = Disjunction {std::move(left_subtree), std::move(right_subtree)};
        
        return Tree {std::move(conj)};
    }

    auto visitPlImplication(PropositionalLogicParser::PlImplicationContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto conj = Implication {std::move(left_subtree), std::move(right_subtree)};
        
        return Tree {std::move(conj)};
    }

    auto visitPlIff(PropositionalLogicParser::PlIffContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        auto left_subtree = std::any_cast<Tree>(left);
        auto right_subtree = std::any_cast<Tree>(right);
        auto conj = Equivalence {std::move(left_subtree), std::move(right_subtree)};
        
        return Tree {std::move(conj)};
    }
};

Formula::Formula(std::string formula) {
    auto input_stream = antlr4::ANTLRInputStream(formula);
    auto lexer = PropositionalLogicLexer(&input_stream);
    auto token_stream = antlr4::CommonTokenStream(&lexer);
    auto parser = PropositionalLogicParser(&token_stream);
    auto builder = FormulaBuilder {};
    auto output = builder.visit(parser.start());
    auto root = std::any_cast<Tree>(output);

    this->m_root = std::make_unique<Tree>(std::move(root));
}