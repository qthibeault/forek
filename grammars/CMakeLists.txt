find_package(ANTLR REQUIRED)

# Create re-useable function to compile an ANTLR grammar into an OBJECT add_library
#
# Accepts as input the name of the grammar to use and optionally the name of the library
# to generate. Compiles the lexer and then the parser for each grammar and adds their
# sources to a library target.
function(PARSER grammar_name)
    set(LEXER ${grammar_name}Lexer)
    set(PARSER ${grammar_name}Parser)

    antlr_target(${LEXER} ${LEXER}.g4 LEXER
        PACKAGE forek)

    antlr_target(${PARSER} ${PARSER}.g4 PARSER
        VISITOR
        PACKAGE forek
        DEPENDS_ANTLR ${LEXER}
        COMPILE_FLAGS -lib ${ANTLR_${LEXER}_OUTPUT_DIR} -DtokenVocab=${LEXER})

    cmake_parse_arguments(PARSER_PREFIX "" "LIBRARY_NAME" "" ${ARGN})
    string(LENGTH ${PARSER_PREFIX_LIBRARY_NAME} LIBRARY_NAME_LEN)

    if(${LIBRARY_NAME_LEN})
        set(LIBRARY_NAME ${PARSER_PREFIX_LIBRARY_NAME})
    else()
        set(LIBRARY_NAME ${grammar_name})
    endif()

    add_library(${LIBRARY_NAME} OBJECT
        ${ANTLR_${LEXER}_CXX_OUTPUTS} ${ANTLR_${PARSER}_CXX_OUTPUTS})

    target_include_directories(${LIBRARY_NAME} PUBLIC
        ${ANTLR4_INCLUDE_DIRS}
        ${ANTLR_${LEXER}_OUTPUT_DIR}
        ${ANTLR_${PARSER}_OUTPUT_DIR})

    target_link_libraries(${LIBRARY_NAME} PRIVATE
        antlr4_static)
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

