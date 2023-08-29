#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "forek/mtl.h"
#include "forek/stl.h"

TEMPLATE_TEST_CASE("Bounded Next", "[mtl]", forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("X[0,1.0] p"));
    REQUIRE_NOTHROW(TestType("next[0.1, 1] p"));
}

TEMPLATE_TEST_CASE("Bounded Globally", "[mtl]", forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("G[0,1.0] p"));
    REQUIRE_NOTHROW(TestType("always[0.1, 1] p"));
}

TEMPLATE_TEST_CASE("Bounded Finally", "[mtl]", forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("F[0,1.0] p"));
    REQUIRE_NOTHROW(TestType("eventually[0.1, 1] p"));
}

TEMPLATE_TEST_CASE("Bounded Release", "[mtl]", forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("p R[0,1.0] q"));
    REQUIRE_NOTHROW(TestType("p release[0.1, 1] q"));
}

TEMPLATE_TEST_CASE("Bounded Until", "[mtl]", forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("p U[0,1.0] q"));
    REQUIRE_NOTHROW(TestType("p until[0.1, 1] q"));
}

TEMPLATE_TEST_CASE("MTL Composition", "[ltl]", forek::mtl::Formula, forek::stl::Formula) {
    REQUIRE_NOTHROW(TestType("G[0.0, 1.0] (not a) and eventually (b -> next[0,1.2] c) or (!d R e) && (f U g)"));
}
