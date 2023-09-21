#include <iostream>
#include <memory>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include "forek/interval.h"
#include "forek/pl/formula.h"
#include "forek/ltl/formula.h"
#include "forek/mtl/formula.h"
#include "forek/stl/formula.h"
#include "forek/stl/visitor.h"

namespace py = pybind11;

using forek::algebra::Comparison;
using forek::algebra::Expr;
using forek::interval::Endpoint;
using forek::interval::Interval;

namespace forek::algebra {

class PyExpr {
   public:
    explicit PyExpr(Expr inner) : m_inner{std::move(inner)} {}

    [[nodiscard]]
    auto evaluate(py::object visitor) const -> py::object;

   private:
    Expr m_inner;
};

}  // namespace forek::algebra

class ExprVisitorWrapper : public forek::algebra::Visitor<py::object> {
   public:
    explicit ExprVisitorWrapper(py::object visitor) : m_visitor{std::move(visitor)} {}

    auto visit_value(double value) -> py::object override {
        auto method = m_visitor.attr("visit_value");
        return method(value);
    }

    auto visit_variable(std::string_view name) -> py::object override {
        auto method = m_visitor.attr("visit_variable");
        return method(std::string(name));
    }

    auto visit_addition(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_addition");
        return method(lhs, rhs);
    }

    auto visit_subtraction(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_subtraction");
        return method(lhs, rhs);
    }

    auto visit_multiplication(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_multiplication");
        return method(lhs, rhs);
    }

    auto visit_division(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_division");
        return method(lhs, rhs);
    }

    auto visit_modulo(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_modulo");
        return method(lhs, rhs);
    }

   private:
    py::object m_visitor;
};

namespace forek::algebra {

auto PyExpr::evaluate(py::object visitor) const -> py::object {
    ExprVisitorWrapper evw(visitor);
    return m_inner.evaluate(evw);
}

}  // namespace forek::algebra

class VisitorWrapper : public forek::stl::Visitor<py::object> {
   public:
    explicit VisitorWrapper(py::object visitor) : m_visitor{std::move(visitor)} {}

    auto visit_true() -> py::object override {
        auto method = m_visitor.attr("visit_true");
        return method();
    }

    auto visit_false() -> py::object override {
        auto method = m_visitor.attr("visit_false");
        return method();
    }

    auto visit_proposition(std::string name) -> py::object override {
        auto method = m_visitor.attr("visit_proposition");
        return method(name);
    }

    auto visit_predicate(const Expr &lhs, Comparison cmp, const Expr &rhs) -> py::object override {
        auto method = m_visitor.attr("visit_predicate");
        return method(forek::algebra::PyExpr(lhs), cmp, forek::algebra::PyExpr(rhs));
    }

    auto visit_negation(py::object inner) -> py::object override {
        auto method = m_visitor.attr("visit_conjunction");
        return method(inner);
    }

    auto visit_conjunction(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_conjunction");
        return method(lhs, rhs);
    }

    auto visit_disjunction(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_disjunction");
        return method(lhs, rhs);
    }

    auto visit_implication(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_implication");
        return method(lhs, rhs);
    }

    auto visit_equivalence(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_implication");
        return method(lhs, rhs);
    }

    auto visit_next(py::object inner) -> py::object override {
        auto method = m_visitor.attr("visit_next");
        return method(inner);
    }

    auto visit_bounded_next(const Interval &i, py::object inner) -> py::object override {
        auto method = m_visitor.attr("visit_bounded_next");
        return method(Interval{i}, inner);
    }

    auto visit_globally(py::object inner) -> py::object override {
        auto method = m_visitor.attr("visit_globally");
        return method(inner);
    }

    auto visit_bounded_globally(const Interval &i, py::object inner) -> py::object override {
        auto method = m_visitor.attr("visit_bounded_globally");
        return method(Interval{i}, inner);
    }

    auto visit_finally(py::object inner) -> py::object override {
        auto method = m_visitor.attr("visit_finally");
        return method(inner);
    }

    auto visit_bounded_finally(const Interval &i, py::object inner) -> py::object override {
        auto method = m_visitor.attr("visit_bounded_finally");
        return method(Interval{i}, inner);
    }

    auto visit_release(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_release");
        return method(lhs, rhs);
    }

    auto visit_bounded_release(const Interval &i, py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_bounded_release");
        return method(Interval{i}, lhs, rhs);
    }

    auto visit_until(py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_until");
        return method(lhs, rhs);
    }

    auto visit_bounded_until(const Interval &i, py::object lhs, py::object rhs) -> py::object override {
        auto method = m_visitor.attr("visit_bounded_until");
        return method(Interval{i}, lhs, rhs);
    }

   private:
    py::object m_visitor;
};

namespace forek::pl {

class PyFormula {
   public:
    explicit PyFormula(const std::string& formula)
        : m_formula{std::make_unique<Formula>(std::move(formula))} {}

    [[nodiscard]]
    auto evaluate(const py::object& visitor) const -> py::object {
        VisitorWrapper vw {visitor};
        return m_formula->evaluate(vw);
    }

   private:
    std::unique_ptr<Formula> m_formula;
};

}  // namespace forek::pl

namespace forek::ltl {

class PyFormula {
   public:
    explicit PyFormula(const std::string& formula)
        : m_formula{std::make_unique<Formula>(std::move(formula))} {}

    [[nodiscard]]
    auto evaluate(const py::object& visitor) const -> py::object {
        VisitorWrapper vw {visitor};
        return m_formula->evaluate(vw);
    }

   private:
    std::unique_ptr<Formula> m_formula;
};

}  // namespace forek::ltl

namespace forek::mtl {

class PyFormula {
   public:
    explicit PyFormula(const std::string& formula)
        : m_formula{std::make_unique<Formula>(std::move(formula))} {}

    [[nodiscard]]
    auto evaluate(const py::object& visitor) const -> py::object {
        VisitorWrapper vw {visitor};
        return m_formula->evaluate(vw);
    }

   private:
    std::unique_ptr<Formula> m_formula;
};

}  // namespace forek::mtl

namespace forek::stl {

class PyFormula {
   public:
    explicit PyFormula(const std::string& formula)
        : m_formula{std::make_unique<Formula>(std::move(formula))} {}

    [[nodiscard]]
    auto evaluate(const py::object& visitor) const -> py::object {
        VisitorWrapper vw {visitor};
        return m_formula->evaluate(vw);
    }

   private:
    std::unique_ptr<Formula> m_formula;
};

}  // namespace forek::stl

namespace alg = forek::algebra;
namespace pl = forek::pl;
namespace ltl = forek::ltl;
namespace mtl = forek::mtl;
namespace stl = forek::stl;

PYBIND11_MODULE(_forek, m) {
    m.doc() = "Python bindings for FoReK library";

    py::class_<Endpoint> ep(m, "Endpoint");

    ep.def_static("open", &Endpoint::open, py::arg("value"))
        .def_static("closed", &Endpoint::closed, py::arg("value"))
        .def_property_readonly("value", &Endpoint::value)
        .def_property_readonly("type", &Endpoint::type);

    py::enum_<Endpoint::Type>(ep, "Type")
        .value("Open", Endpoint::Type::Open)
        .value("Closed", Endpoint::Type::Closed);

    py::class_<Interval>(m, "Interval")
        .def_property_readonly("lower", &Interval::lower)
        .def_property_readonly("upper", &Interval::upper);

    py::enum_<Comparison>(m, "Comparison")
        .value("LessThan", Comparison::LessThan)
        .value("LessThanEqual", Comparison::LessThanEqual)
        .value("Equal", Comparison::Equal)
        .value("NotEqual", Comparison::NotEqual)
        .value("GreaterThanEqual", Comparison::GreaterThanEqual)
        .value("GreaterThan", Comparison::GreaterThan);

    py::class_<alg::PyExpr>(m, "Expr")
        .def("evaluate", &alg::PyExpr::evaluate, py::arg("visitor"));

    py::class_<pl::PyFormula>(m, "PLFormula")
        .def(py::init<const std::string&>())
        .def("evaluate", &pl::PyFormula::evaluate, py::arg("visitor"));

    py::class_<ltl::PyFormula>(m, "LTLFormula")
        .def(py::init<const std::string&>())
        .def("evaluate", &ltl::PyFormula::evaluate, py::arg("visitor"));

    py::class_<mtl::PyFormula>(m, "MTLFormula")
        .def(py::init<const std::string&>())
        .def("evaluate", &mtl::PyFormula::evaluate, py::arg("visitor"));

    py::class_<stl::PyFormula>(m, "STLFormula")
        .def(py::init<const std::string&>())
        .def("evaluate", &stl::PyFormula::evaluate, py::arg("visitor"));
}
