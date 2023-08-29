#include <exception>
#include <string>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "forek/errors.h"
#include "forek/ltl.h"
#include "forek/mtl.h"
#include "forek/stl.h"

using forek::errors::LexerError;
using forek::errors::ParserError;

TEMPLATE_TEST_CASE("Next", "[ltl]", forek::ltl::Formula, forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("X p"));
    REQUIRE_NOTHROW(TestType("next p"));
}

TEMPLATE_TEST_CASE("Globally", "[ltl]", forek::ltl::Formula, forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("G p"));
    REQUIRE_NOTHROW(TestType("always p"));
}

TEMPLATE_TEST_CASE("Finally", "[ltl]", forek::ltl::Formula, forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("F p"));
    REQUIRE_NOTHROW(TestType("eventually p"));
}

TEMPLATE_TEST_CASE("Release", "[ltl]", forek::ltl::Formula, forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("p R q"));
    REQUIRE_NOTHROW(TestType("p release q"));
}

TEMPLATE_TEST_CASE("Until", "[ltl]", forek::ltl::Formula, forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("p U q"));
    REQUIRE_NOTHROW(TestType("p until q"));
}

TEMPLATE_TEST_CASE("LTL Composition", "[ltl]", forek::ltl::Formula, forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("G (not a) and eventually (b -> next c) or (!d R e) && (f U g)"));
}
