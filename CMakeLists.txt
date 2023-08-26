cmake_minimum_required(VERSION 3.20)

project(forek
  VERSION 0.0.1
  DESCRIPTION "A toolkit for authoring and parsing Temporal Logic formulae."
  HOMEPAGE_URL "https://github.com/andersonjwan/forek"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++20 features are used in this project

# Disable vender-specific plugins by default.
#
# This is disabled to protect unwarrented linking of other extensions but can be
# overridden on a per-target basis.
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for editor tooling support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# include(ExternalAntlr4Cpp)
include(FetchAntlrJar)

# Locate ANTLR jar file
# set(ANTLR_EXECUTABLE ${CMAKE_SOURCE_DIR}/vendor/antlr-4.13.0-complete.jar)
FetchAntlrJar(VERSION 4.13.0)

# Download fmt library
FetchContent_Declare(fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 10.1.0)

# Download ANTLR4 library
FetchContent_Declare(antlr4
    GIT_REPOSITORY https://github.com/antlr/antlr4
    GIT_TAG 4.13.0)

FetchContent_MakeAvailable(fmtlib antlr4)

add_subdirectory(${antlr4_SOURCE_DIR}/runtime/Cpp)
add_subdirectory(grammars)

add_library(forek STATIC
    src/interval.cxx
    src/pl.cxx
    src/ltl.cxx
    src/mtl.cxx
    src/stl.cxx
    src/tptl.cxx
    src/tqtl.cxx
    src/stpl.cxx)

target_include_directories(forek PUBLIC
    include)

target_link_libraries(forek PRIVATE
    arithmetic_parser
    pl_parser
    ltl_parser
    pt_ltl_parser
    mtl_parser
    stl_parser
    tptl_parser
    tqtl_parser
    stpl_parser
    fmt::fmt-header-only)

enable_testing()
add_subdirectory(tests)
