#include "forek/pl.h"
#include "forek/errors.h"

#include <exception>
#include <string>

#include <catch2/catch_test_macros.hpp>

using forek::errors::LexerError;
using forek::errors::ParserError;
using forek::pl::Formula;

TEST_CASE("Propositional Logic", "[pl]") {
    SECTION("Proposition") {
        REQUIRE_NOTHROW(Formula("p"));
        REQUIRE_NOTHROW(Formula("p_1"));
        REQUIRE_THROWS_AS(Formula("1p"), ParserError);
        REQUIRE_THROWS_AS(Formula("~p"), LexerError);
    }

    SECTION("Negation") {
        REQUIRE_NOTHROW(Formula("!p"));
        REQUIRE_NOTHROW(Formula("not p"));
    }

    SECTION("Conjunction") {
        REQUIRE_NOTHROW(Formula("p && q"));
        REQUIRE_NOTHROW(Formula("p and q"));
    }

    SECTION("Disjunction") {
        REQUIRE_NOTHROW(Formula("p || q"));
        REQUIRE_NOTHROW(Formula("p or q"));
    }

    SECTION("Implication") {
        REQUIRE_NOTHROW(Formula("p -> q"));
        REQUIRE_NOTHROW(Formula("p implies q"));
    }

    SECTION("Equivalence") {
        REQUIRE_NOTHROW(Formula("p <-> q"));
        REQUIRE_NOTHROW(Formula("p iff q"));
    }

    SECTION("Composition") {
        REQUIRE_NOTHROW(Formula("not a and (b -> c) or (!d <-> e)"));
    }
}
