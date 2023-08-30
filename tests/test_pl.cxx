#include <exception>
#include <string>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "forek/errors.h"
#include "forek/ltl.h"
#include "forek/mtl.h"
#include "forek/pl.h"
#include "forek/stl.h"

using forek::errors::LexerError;
using forek::errors::ParserError;

TEMPLATE_TEST_CASE("Proposition", "[pl]", forek::pl::Formula, forek::ltl::Formula,
                   forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("p"));
    REQUIRE_NOTHROW(TestType("p_1"));
    REQUIRE_THROWS_AS(TestType("1p"), ParserError);
    REQUIRE_THROWS_AS(TestType("~p"), LexerError);
}

TEMPLATE_TEST_CASE("Negation", "[pl]", forek::pl::Formula, forek::ltl::Formula, forek::mtl::Formula,
                   forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("!p"));
    REQUIRE_NOTHROW(TestType("not p"));
}

TEMPLATE_TEST_CASE("Conjunction", "[pl]", forek::pl::Formula, forek::ltl::Formula,
                   forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("p && q"));
    REQUIRE_NOTHROW(TestType("p and q"));
}

TEMPLATE_TEST_CASE("Disjunction", "[pl]", forek::pl::Formula, forek::ltl::Formula,
                   forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("p || q"));
    REQUIRE_NOTHROW(TestType("p or q"));
}

TEMPLATE_TEST_CASE("Implication", "[pl]", forek::pl::Formula, forek::ltl::Formula,
                   forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("p -> q"));
    REQUIRE_NOTHROW(TestType("p implies q"));
}

TEMPLATE_TEST_CASE("Equivalence", "[pl]", forek::pl::Formula, forek::ltl::Formula,
                   forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("p <-> q"));
    REQUIRE_NOTHROW(TestType("p iff q"));
}

TEMPLATE_TEST_CASE("PL Composition", "[pl]", forek::pl::Formula, forek::ltl::Formula,
                   forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("not a and (b -> c) or (!d <-> e)"));
}

TEMPLATE_TEST_CASE("PL String Conversion", "[pl]", forek::pl::Formula) {
    auto f1 = TestType("not a and (b -> c) or (!d <-> e)");
    auto str = std::string{f1};
    auto f2 = TestType(str);

    REQUIRE(f1 == f2);
}
