#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "forek/errors.h"
#include "forek/stl.h"

using forek::errors::ParserError;

TEMPLATE_TEST_CASE("Predicate", "[stl]", forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("1.0 + 2.6 * x <= 21.7"));
    REQUIRE_NOTHROW(TestType("x * y + z > a / b"));
    REQUIRE_THROWS_AS(TestType("1 + 2"), ParserError);
}

TEMPLATE_TEST_CASE("STL Composition", "[stl]", forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("G[0, 1] (x < 1) and eventually (not (y >= 3 * z) -> z >= 10)"));
}
