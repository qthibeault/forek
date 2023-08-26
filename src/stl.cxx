#include "forek/algebra.h"
#include "forek/interval.h"
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
using forek::interval::Interval;
using forek::interval::make_exclusive;
using forek::interval::make_inclusive;
using forek::ir::BoundedFinally;
using forek::ir::BoundedGlobally;
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

template <size_t N>
auto parse_interval_value(SignalTemporalLogicParser::IntervalContext *ctx) -> double {
    return ctx->Infinity(N) ? std::numeric_limits<double>::infinity()
                            : std::stod(ctx->Scalar(N)->getText());
}

class Builder : public SignalTemporalLogicParserBaseVisitor {
    using Parser = SignalTemporalLogicParser;

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

    auto visitArithmeticParentheses(Parser::ArithmeticParenthesesContext *ctx) -> std::any override {
        return visit(ctx->expression());
    }

    virtual std::any visitArithmeticTerm(
        SignalTemporalLogicParser::ArithmeticTermContext *context) = 0;

    virtual std::any visitArithmeticPlus(
        SignalTemporalLogicParser::ArithmeticPlusContext *context) = 0;

    virtual std::any visitArithmeticVariable(
        SignalTemporalLogicParser::ArithmeticVariableContext *context) = 0;

    virtual std::any visitArithmeticConstant(
        SignalTemporalLogicParser::ArithmeticConstantContext *context) = 0;

    virtual std::any visitRelationalOperator(
        SignalTemporalLogicParser::RelationalOperatorContext *context) = 0;

    virtual std::any visitPredicate(SignalTemporalLogicParser::PredicateContext *context) = 0;

    virtual std::any visitProposition(SignalTemporalLogicParser::PropositionContext *context) = 0;

    virtual std::any visitPlTrue(SignalTemporalLogicParser::PlTrueContext *context) = 0;

    virtual std::any visitPlFalse(SignalTemporalLogicParser::PlFalseContext *context) = 0;

    virtual std::any visitPlProposition(
        SignalTemporalLogicParser::PlPropositionContext *context) = 0;

    virtual std::any visitStlPredicate(SignalTemporalLogicParser::StlPredicateContext *context) = 0;

    auto visitPlNegation(SignalTemporalLogicParser::PlNegationContext *ctx) -> std::any override {
        return nullptr;
    }

    virtual std::any visitPlConjunction(
        SignalTemporalLogicParser::PlConjunctionContext *context) = 0;

    virtual std::any visitPlDisjunction(
        SignalTemporalLogicParser::PlDisjunctionContext *context) = 0;

    virtual std::any visitPlImplication(
        SignalTemporalLogicParser::PlImplicationContext *context) = 0;

    virtual std::any visitPlIff(SignalTemporalLogicParser::PlIffContext *context) = 0;

    virtual std::any visitLtlUntil(SignalTemporalLogicParser::LtlUntilContext *context) = 0;

    virtual std::any visitLtlAlways(SignalTemporalLogicParser::LtlAlwaysContext *context) = 0;

    virtual std::any visitLtlRelease(SignalTemporalLogicParser::LtlReleaseContext *context) = 0;

    virtual std::any visitLtlEventually(
        SignalTemporalLogicParser::LtlEventuallyContext *context) = 0;

    virtual std::any visitLtlNext(SignalTemporalLogicParser::LtlNextContext *context) = 0;
};

auto parse_formula(std::string formula) -> TreePtr {
    auto input_stream = antlr4::ANTLRInputStream(formula);
    auto lexer = SignalTemporalLogicLexer(&input_stream);
    auto token_stream = antlr4::CommonTokenStream(&lexer);
    auto parser = SignalTemporalLogicParser(&token_stream);
    auto builder = Builder{};
    auto output = builder.visit(parser.start());

    return std::any_cast<TreePtr>(output);
}

Formula::Formula(std::string formula) : m_root{parse_formula(std::move(formula))} {}
Formula::Formula(Tree root) : m_root{std::make_shared<Tree>(std::move(root))} {}
Formula::Formula(std::shared_ptr<Tree> root) : m_root{std::move(root)} {}
