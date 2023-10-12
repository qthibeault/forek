cmake_minimum_required(VERSION 3.20)

project(forek
  VERSION 0.1.0
  DESCRIPTION "A toolkit for authoring and parsing Temporal Logic formulae."
  HOMEPAGE_URL "https://github.com/andersonjwan/forek"
  LANGUAGES CXX
)

# Create compile_commands.json for editor tooling support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ensure code is compiled with PIC flag
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Option to enable testing for FoRek library
option(FOREK_ENABLE_TESTS "Enable testing FoRek library" ON)

# Option to create python bindings
option(FOREK_PYTHON_BINDINGS "Enable compiling python bindings" OFF)

# Enable FetchContent module to download dependencies
include(FetchContent)

# Ensure Git is installed before fetching dependencies
find_package(Git REQUIRED)

# Download fmt library
FetchContent_Declare(fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 10.1.0)

FetchContent_MakeAvailable(fmtlib)

add_subdirectory(grammars)

add_library(forek STATIC
    src/interval.cxx
    src/algebra.cxx
    src/common.cxx
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
    forek::parsers
    fmt::fmt-header-only)

target_compile_features(forek PUBLIC cxx_std_17)

if (MSVC)
    # warning level 4
    target_compile_options(forek PRIVATE /Wall /WX)
else()
    # additional warnings
    target_compile_options(forek PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_library(forek::forek ALIAS forek)

if(FOREK_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(FOREK_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()
