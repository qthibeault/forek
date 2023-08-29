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
using forek::interval::Interval;
using forek::interval::make_exclusive;
using forek::interval::make_inclusive;
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
using forek::stl::Formula;
using forek::stl::Tree;

using TreePtr = std::shared_ptr<Tree>;
using ExprPtr = std::shared_ptr<Expr>;

template <size_t N>
auto parse_interval_value(SignalTemporalLogicParser::IntervalContext *ctx) -> double {
    return ctx->Infinity(N) ? std::numeric_limits<double>::infinity()
                            : std::stod(ctx->Scalar(N)->getText());
}

class STLBuilder : public SignalTemporalLogicParserBaseVisitor {
    using Parser = SignalTemporalLogicParser;

   public:
    auto visitStart(Parser::StartContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitParentheses(Parser::ParenthesesContext *ctx) -> std::any override {
        return visit(ctx->formula());
    }

    auto visitInterval(Parser::IntervalContext *ctx) -> std::any override {
        auto lval = parse_interval_value<0>(ctx);
        auto uval = parse_interval_value<1>(ctx);
        auto lower = ctx->LeftBracket() ? make_inclusive(lval) : make_exclusive(lval);
        auto upper = ctx->RightBracket() ? make_inclusive(uval) : make_exclusive(uval);

        return Interval{lower, upper};
    }

    auto visitArithmeticParentheses(Parser::ArithmeticParenthesesContext *ctx)
        -> std::any override {
        return visit(ctx->expression());
    }

    auto visitArithmeticTerm(Parser::ArithmeticTermContext *ctx) -> std::any override {
        return visit(ctx->term());
    }

    auto visitArithmeticPlus(Parser::ArithmeticPlusContext *ctx) -> std::any override {
        auto left = visit(ctx->expression(0));
        auto right = visit(ctx->expression(1));

        return std::make_shared<Expr>(
            Add<Expr>{std::any_cast<ExprPtr>(left), std::any_cast<ExprPtr>(right)});
    }

    auto visitArithmeticMinus(Parser::ArithmeticMinusContext *ctx) -> std::any override {
        auto left = visit(ctx->expression(0));
        auto right = visit(ctx->expression(1));

        return std::make_shared<Expr>(
            Sub<Expr>{std::any_cast<ExprPtr>(left), std::any_cast<ExprPtr>(right)});
    }

    auto visitArithmeticTimes(Parser::ArithmeticTimesContext *ctx) -> std::any override {
        auto left = visit(ctx->expression(0));
        auto right = visit(ctx->expression(1));

        return std::make_shared<Expr>(
            Mult<Expr>{std::any_cast<ExprPtr>(left), std::any_cast<ExprPtr>(right)});
    }

    auto visitArithmeticDivide(Parser::ArithmeticDivideContext *ctx) -> std::any override {
        auto left = visit(ctx->expression(0));
        auto right = visit(ctx->expression(1));

        return std::make_shared<Expr>(
            Div<Expr>{std::any_cast<ExprPtr>(left), std::any_cast<ExprPtr>(right)});
    }

    auto visitArithmeticModulus(Parser::ArithmeticModulusContext *ctx) -> std::any override {
        auto left = visit(ctx->expression(0));
        auto right = visit(ctx->expression(1));

        return std::make_shared<Expr>(
            Mod<Expr>{std::any_cast<ExprPtr>(left), std::any_cast<ExprPtr>(right)});
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

    auto visitPlTrue(Parser::PlTrueContext *ctx) -> std::any override {
        return std::make_shared<Tree>(True{});
    }

    auto visitPlFalse(Parser::PlFalseContext *ctx) -> std::any override {
        return std::make_shared<Tree>(False{});
    }

    auto visitPlProposition(Parser::PlPropositionContext *ctx) -> std::any override {
        return visit(ctx->proposition());
    }

    auto visitStlPredicate(Parser::StlPredicateContext *ctx) -> std::any override {
        return visit(ctx->predicate());
    }

    auto visitPlNegation(SignalTemporalLogicParser::PlNegationContext *ctx) -> std::any override {
        return nullptr;
    }

    auto visitPlConjunction(Parser::PlConjunctionContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        return std::make_shared<Tree>(
            Conjunction<Tree>{std::any_cast<TreePtr>(left), std::any_cast<TreePtr>(right)});
    }

    auto visitPlDisjunction(Parser::PlDisjunctionContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        return std::make_shared<Tree>(
            Disjunction<Tree>{std::any_cast<TreePtr>(left), std::any_cast<TreePtr>(right)});
    }

    auto visitPlImplication(Parser::PlImplicationContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        return std::make_shared<Tree>(
            Implication<Tree>{std::any_cast<TreePtr>(left), std::any_cast<TreePtr>(right)});
    }

    auto visitPlIff(Parser::PlIffContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));

        return std::make_shared<Tree>(
            Equivalence<Tree>{std::any_cast<TreePtr>(left), std::any_cast<TreePtr>(right)});
    }

    auto visitLtlNext(Parser::LtlNextContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto p_interval = ctx->interval();

        if (p_interval) {
            auto interval = visit(p_interval);

            return std::make_shared<Tree>(BoundedNext<Tree>{std::any_cast<Interval>(interval),
                                                            std::any_cast<TreePtr>(inner)});
        }

        return std::make_shared<Tree>(Next<Tree>{std::any_cast<TreePtr>(inner)});
    }

    auto visitLtlAlways(Parser::LtlAlwaysContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto p_interval = ctx->interval();

        if (p_interval) {
            auto interval = visit(p_interval);

            return std::make_shared<Tree>(BoundedGlobally<Tree>{std::any_cast<Interval>(interval),
                                                                std::any_cast<TreePtr>(inner)});
        }

        return std::make_shared<Tree>(Globally<Tree>{std::any_cast<TreePtr>(inner)});
    }

    auto visitLtlEventually(Parser::LtlEventuallyContext *ctx) -> std::any override {
        auto inner = visit(ctx->formula());
        auto p_interval = ctx->interval();

        if (p_interval) {
            auto interval = visit(p_interval);

            return std::make_shared<Tree>(BoundedFinally<Tree>{std::any_cast<Interval>(interval),
                                                               std::any_cast<TreePtr>(inner)});
        }

        return std::make_shared<Tree>(Finally<Tree>{std::any_cast<TreePtr>(inner)});
    }

    auto visitLtlUntil(Parser::LtlUntilContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto p_interval = ctx->interval();

        if (p_interval) {
            auto interval = visit(p_interval);

            return std::make_shared<Tree>(BoundedUntil<Tree>{std::any_cast<Interval>(interval),
                                                             std::any_cast<TreePtr>(left),
                                                             std::any_cast<TreePtr>(right)});
        }

        return std::make_shared<Tree>(
            Until<Tree>{std::any_cast<TreePtr>(left), std::any_cast<TreePtr>(right)});
    }

    auto visitLtlRelease(Parser::LtlReleaseContext *ctx) -> std::any override {
        auto left = visit(ctx->formula(0));
        auto right = visit(ctx->formula(1));
        auto p_interval = ctx->interval();

        if (p_interval) {
            auto interval = visit(p_interval);

            return std::make_shared<Tree>(BoundedRelease<Tree>{std::any_cast<Interval>(interval),
                                                               std::any_cast<TreePtr>(left),
                                                               std::any_cast<TreePtr>(right)});
        }

        return std::make_shared<Tree>(
            Release<Tree>{std::any_cast<TreePtr>(left), std::any_cast<TreePtr>(right)});
    }
};

auto parse_stl_formula(std::string formula) -> TreePtr {
    auto input_stream = antlr4::ANTLRInputStream(formula);
    auto lexer = SignalTemporalLogicLexer(&input_stream);
    auto token_stream = antlr4::CommonTokenStream(&lexer);
    auto parser = SignalTemporalLogicParser(&token_stream);
    auto builder = STLBuilder{};
    auto output = builder.visit(parser.start());

    return std::any_cast<TreePtr>(output);
}

Formula::Formula(std::string formula) : m_root{parse_stl_formula(std::move(formula))} {}
Formula::Formula(Tree root) : m_root{std::make_shared<Tree>(std::move(root))} {}
Formula::Formula(std::shared_ptr<Tree> root) : m_root{std::move(root)} {}
