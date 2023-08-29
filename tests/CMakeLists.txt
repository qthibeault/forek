include(FetchContent)
include(CTest)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

add_executable(test_pl test_pl.cxx)
target_link_libraries(test_pl PRIVATE forek Catch2::Catch2WithMain)

add_executable(test_ltl test_ltl.cxx)
target_link_libraries(test_ltl PRIVATE forek Catch2::Catch2WithMain)

catch_discover_tests(test_pl)
catch_discover_tests(test_ltl)
