# Include CMake module for downloading content
include(FetchContent)

# Ensure Git is installed
find_package(Git REQUIRED)

# Define ANTLR4 jar name in one place
set(ANTLR4_JAR_NAME antlr-4.13.0-complete.jar)

# Set ANTLR4 build options
set(WITH_DEMO OFF)
set(ANTLR_BUILD_CPP_TESTS OFF)

# Download ANTLR4 library
FetchContent_Declare(antlr4
    GIT_REPOSITORY https://github.com/antlr/antlr4
    GIT_TAG 4.13.0
    SOURCE_SUBDIR runtime/Cpp)

# Download ANTLR4 Jar for processing grammar files
FetchContent_Declare(antlr4-jar
    URL https://www.antlr.org/download/${ANTLR4_JAR_NAME}
    DOWNLOAD_NO_EXTRACT TRUE)

FetchContent_MakeAvailable(antlr4 antlr4-jar)

# Include ANTLR4 CMake modules
list(APPEND CMAKE_MODULE_PATH ${antlr4_SOURCE_DIR}/runtime/Cpp/cmake)

# Ensure ANTLR4 is compiled with C++14
target_compile_features(antlr4_static PUBLIC cxx_std_17)

set_target_properties(antlr4_shared PROPERTIES
    EXCLUDE_FROM_ALL TRUE)

# Register ANTLR4 JAR
set(ANTLR_EXECUTABLE ${antlr4-jar_SOURCE_DIR}/${ANTLR4_JAR_NAME})

# Locate ANTLR4 Package
find_package(ANTLR REQUIRED)

# Create re-useable function to compile an ANTLR grammar into an OBJECT add_library
#
# Accepts as input the name of the grammar to use and optionally the name of the library
# to generate. Compiles the lexer and then the parser for each grammar and adds their
# sources to a library target.
function(PARSER grammar_name)
    set(LEXER ${grammar_name}Lexer)
    set(PARSER ${grammar_name}Parser)

    cmake_parse_arguments(PARSER_PREFIX "" "LIBRARY_NAME" "" ${ARGN})
    string(LENGTH ${PARSER_PREFIX_LIBRARY_NAME} LIBRARY_NAME_LEN)

    antlr_target(${LEXER} ${LEXER}.g4 LEXER
        PACKAGE forek)

    antlr_target(${PARSER} ${PARSER}.g4 PARSER
        VISITOR
        PACKAGE forek
        DEPENDS_ANTLR ${LEXER}
        COMPILE_FLAGS -lib ${ANTLR_${LEXER}_OUTPUT_DIR} -DtokenVocab=${LEXER})

    if(${LIBRARY_NAME_LEN})
        set(LIBRARY_NAME ${PARSER_PREFIX_LIBRARY_NAME})
    else()
        set(LIBRARY_NAME ${grammar_name})
    endif()

    add_library(${LIBRARY_NAME} OBJECT
        ${ANTLR_${LEXER}_CXX_OUTPUTS} ${ANTLR_${PARSER}_CXX_OUTPUTS})

    target_include_directories(${LIBRARY_NAME} PUBLIC
        ${antlr4_SOURCE_DIR}/runtime/Cpp/runtime/src
        ${ANTLR_${LEXER}_OUTPUT_DIR}
        ${ANTLR_${PARSER}_OUTPUT_DIR})

    target_link_libraries(${LIBRARY_NAME} PRIVATE
        antlr4_static)

    target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_17)
endfunction(PARSER)

PARSER(Arithmetic LIBRARY_NAME arithmetic_parser)
PARSER(PropositionalLogic LIBRARY_NAME pl_parser)
PARSER(LinearTemporalLogic LIBRARY_NAME ltl_parser)
PARSER(PastTimeLinearTemporalLogic LIBRARY_NAME pt_ltl_parser)
PARSER(MetricTemporalLogic LIBRARY_NAME mtl_parser)
PARSER(SignalTemporalLogic LIBRARY_NAME stl_parser)
PARSER(TimedPropositionalTemporalLogic LIBRARY_NAME tptl_parser)
PARSER(TimedQualityTemporalLogic LIBRARY_NAME tqtl_parser)
PARSER(SpatioTemporalPerceptionLogic LIBRARY_NAME stpl_parser)
