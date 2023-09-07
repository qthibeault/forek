#include <memory>

#include "forek/algebra.h"
#include "forek/interval.h"
#include "forek/pl/ir.h"
#include "forek/stl/formula.h"
#include "forek/stl/ir.h"
#include "forek/stl/tree.h"

#include "SignalTemporalLogicLexer.h"
#include "SignalTemporalLogicParser.h"
#include "SignalTemporalLogicParserBaseVisitor.h"
#include "common.h"
#include "listeners.h"

#include "antlr4-runtime.h"

using forek::SignalTemporalLogicLexer;
using forek::SignalTemporalLogicParser;
using forek::SignalTemporalLogicParserBaseVisitor;
using forek::algebra::Add;
using forek::algebra::Comparison;
using forek::algebra::Div;
using forek::algebra::Expr;
using forek::algebra::Literal;
using forek::algebra::Mod;
using forek::algebra::Mult;
using forek::algebra::Sub;
using forek::algebra::Variable;
using forek::common::make_binary;
using forek::common::make_interval;
using forek::common::make_unary;
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
using forek::ir::Predicate;
using forek::ir::Proposition;
using forek::ir::Release;
using forek::ir::True;
using forek::ir::Until;
using forek::listeners::LexerErrorListener;
using forek::listeners::ParserErrorListener;
using forek::stl::Formula;
using forek::stl::Tree;

using TreePtr = std::shared_ptr<Tree>;
using ExprPtr = std::shared_ptr<Expr>;

namespace forek::stl {

class Builder : public SignalTemporalLogicParserBaseVisitor {
    using Parser = SignalTemporalLogicParser;

   public:
    auto visitStart(Parser::StartContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitParentheses(Parser::ParenthesesContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitInterval(Parser::IntervalContext *ctx) -> std::any override {
        return make_interval(ctx);
    }

    auto visitArithmeticParentheses(Parser::ArithmeticParenthesesContext *ctx)
        -> std::any override {
        return visit(ctx->expression());
    }

    auto visitArithmeticTerm(Parser::ArithmeticTermContext *ctx) -> std::any override {
        return visit(ctx->term());
    }

    auto visitArithmeticPlus(Parser::ArithmeticPlusContext *ctx) -> std::any override {
        return make_binary<Add, Expr>(visit(ctx->expression(0)), visit(ctx->expression(1)));
    }

    auto visitArithmeticMinus(Parser::ArithmeticMinusContext *ctx) -> std::any override {
        return make_binary<Sub, Expr>(visit(ctx->expression(0)), visit(ctx->expression(1)));
    }

    auto visitArithmeticTimes(Parser::ArithmeticTimesContext *ctx) -> std::any override {
        return make_binary<Mult, Expr>(visit(ctx->expression(0)), visit(ctx->expression(1)));
    }

    auto visitArithmeticDivide(Parser::ArithmeticDivideContext *ctx) -> std::any override {
        return make_binary<Div, Expr>(visit(ctx->expression(0)), visit(ctx->expression(1)));
    }

    auto visitArithmeticModulus(Parser::ArithmeticModulusContext *ctx) -> std::any override {
        return make_binary<Mod, Expr>(visit(ctx->expression(0)), visit(ctx->expression(1)));
    }

    auto visitArithmeticVariable(Parser::ArithmeticVariableContext *ctx) -> std::any override {
        return std::make_shared<Expr>(Variable{ctx->Identifier()->getText()});
    }

    auto visitArithmeticConstant(Parser::ArithmeticConstantContext *ctx) -> std::any override {
        return std::make_shared<Expr>(Literal{std::stod(ctx->Scalar()->getText())});
    }

    auto visitRelationalOperator(Parser::RelationalOperatorContext *ctx) -> std::any override {
        auto raw = ctx->getText();

        if (raw == "<") {
            return Comparison::LessThan;
        }
        if (raw == "<=") {
            return Comparison::LessThanEqual;
        }
        if (raw == "==") {
            return Comparison::Equal;
        }
        if (raw == ">=") {
            return Comparison::GreaterThanEqual;
        }
        if (raw == ">") {
            return Comparison::GreaterThan;
        }
        if (raw == "!=") {
            return Comparison::NotEqual;
        }

        throw std::runtime_error{"Unknown comparison operator"};
    }

    auto visitPredicate(Parser::PredicateContext *ctx) -> std::any override {
        auto op = visit(ctx->relationalOperator());
        auto left = visit(ctx->expression(0));
        auto right = visit(ctx->expression(1));

        return std::make_shared<Tree>(Predicate{std::any_cast<ExprPtr>(left),
                                                std::any_cast<Comparison>(op),
                                                std::any_cast<ExprPtr>(right)});
    }

    auto visitProposition(Parser::PropositionContext *ctx) -> std::any override {
        return std::make_shared<Tree>(Proposition{ctx->Identifier()->getText()});
    }

    auto visitPlTrue(Parser::PlTrueContext *) -> std::any override {
        return std::make_shared<Tree>(True{});
    }

    auto visitPlFalse(Parser::PlFalseContext *) -> std::any override {
        return std::make_shared<Tree>(False{});
    }

    auto visitPlProposition(Parser::PlPropositionContext *ctx) -> std::any override {
        return visit(ctx->proposition());
    }

    auto visitStlPredicate(Parser::StlPredicateContext *ctx) -> std::any override {
        return visit(ctx->predicate());
    }

    auto visitPlNegation(SignalTemporalLogicParser::PlNegationContext *ctx) -> std::any override {
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

    auto visitLtlNext(Parser::LtlNextContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto interval = ctx->interval();

        if (interval) {
            return make_unary<BoundedNext, Tree>(visit(interval), inner);
        }

        return make_unary<Next, Tree>(inner);
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

    auto visitLtlUntil(Parser::LtlUntilContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto interval = ctx->interval();

        if (interval) {
            return make_binary<BoundedUntil, Tree>(visit(interval), left, right);
        }

        return make_binary<Until, Tree>(left, right);
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
};

auto parse_formula(std::string_view formula) -> TreePtr {
    auto input_stream = antlr4::ANTLRInputStream(formula);
    auto lexer = SignalTemporalLogicLexer(&input_stream);
    auto lexer_listener = std::make_unique<LexerErrorListener>();

    lexer.removeErrorListeners();
    lexer.addErrorListener(lexer_listener.get());

    auto token_stream = antlr4::CommonTokenStream(&lexer);
    auto parser = SignalTemporalLogicParser(&token_stream);
    auto parser_listener = std::make_unique<ParserErrorListener>();

    parser.removeErrorListeners();
    parser.addErrorListener(parser_listener.get());

    auto builder = Builder{};
    auto output = builder.visit(parser.start());

    return std::any_cast<TreePtr>(output);
}

Formula::Formula(std::string_view formula) : m_root{parse_formula(std::move(formula))} {}
Formula::Formula(Tree root) : m_root{std::make_shared<Tree>(std::move(root))} {}
Formula::Formula(std::shared_ptr<Tree> root) : m_root{std::move(root)} {}

}  // namespace forek::stl

