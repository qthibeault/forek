cmake_minimum_required(VERSION 3.20)

project(forek
  VERSION 0.1.0
  DESCRIPTION "A toolkit for authoring and parsing Temporal Logic formulae."
  HOMEPAGE_URL "https://github.com/andersonjwan/forek"
  LANGUAGES CXX)

# Create compile_commands.json for editor tooling support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Option to enable testing for FoRek library
option(BUILD_TESTS "Build FoRek library unit tests" ON)

# Enable building python bindings
option(BUILD_PYTHON "Build FoReK python bindings" ON)

add_subdirectory(src)
add_subdirectory(python EXCLUDE_FROM_ALL)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
