#include "forek/pl.h"

#include <exception>
#include <string>

#include <catch2/catch_test_macros.hpp>

using forek::pl::Formula;

auto try_parse(std::string formula) -> bool {
    try {
        Formula(std::move(formula));
        return true;
    } catch (std::exception& ex) {
        return false;
    }
}

TEST_CASE("Propositional Logic", "[pl]") {
    SECTION("Requirement 1") {
        static std::string req =
            "p";

        REQUIRE(try_parse(req));
    }

    SECTION("Requirement 2") {
        static std::string req =
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q)";

        REQUIRE(try_parse(req));
    }

    SECTION("Requirement 3") {
        static std::string req =
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) && "
            "not (p) && (not q) && not (p) && (not q)";

        REQUIRE(try_parse(req));
    }

    SECTION("Requirement 4") {
        static std::string req =
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) or not (p) && (not q)";

        REQUIRE(try_parse(req));
    }

    SECTION("Requirement 5") {
        static std::string req =
            "not (p) && (not q) & not (p) && (not q) & not (p) && (not q) or not (p) && (not q) and "
            "not (p) && (not q)";

        REQUIRE(try_parse(req));
    }

    SECTION("Requirement 6") {
        static std::string req =
            "not (p) && (not q) && not (p) && (not q) && not (p) && (not q) or not (p) && (not q) and "
            "not (p) && (not q) implies not (p) && (not q)";

        REQUIRE(try_parse(req));
    }
}
