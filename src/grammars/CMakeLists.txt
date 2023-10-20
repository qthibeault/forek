# Include CMake module for downloading content
include(FetchContent)

# Ensure Git is installed
find_package(Git REQUIRED)

# Define ANTLR4 jar name in one place
set(ANTLR4_JAR_NAME antlr-4.13.0-complete.jar)

# Do not enable building ANTLR4 runtime shared library
set(ANTLR_BUILD_SHARED FALSE)

# Do not build ANTLR4 unit tests
set(ANTLR_BUILD_CPP_TESTS FALSE)

# Download ANTLR4 Jar for processing grammar files
FetchContent_Declare(antlr4-jar
    URL https://www.antlr.org/download/${ANTLR4_JAR_NAME}
    DOWNLOAD_NO_EXTRACT TRUE)

FetchContent_MakeAvailable(antlr4-jar)

# Download ANTLR4 library
FetchContent_Declare(antlr4
    GIT_REPOSITORY https://github.com/antlr/antlr4
    GIT_TAG 4.13.0)

# Until CMake 3.28 is released, this is the way to exclude imported project targets
FetchContent_GetProperties(antlr4)

if(NOT antlr4_POPULATED)
    FetchContent_Populate(antlr4)
    add_subdirectory(${antlr4_SOURCE_DIR}/runtime/Cpp ${antlr4_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Set location of Antlr4 JAR file
set(ANTLR_EXECUTABLE ${antlr4-jar_SOURCE_DIR}/${ANTLR4_JAR_NAME})

# Include ANTLR4 CMake modules
list(APPEND CMAKE_MODULE_PATH ${antlr4_SOURCE_DIR}/runtime/Cpp/cmake)

# Locate ANTLR4 Package
find_package(ANTLR REQUIRED)

# Ensure ANTLR4 is compiled with C++14
target_compile_features(antlr4_static PUBLIC cxx_std_14)

add_library(forek_grammars OBJECT ${GRAMMAR_SOURCES})

target_include_directories(forek_grammars
    PUBLIC ${antlr4_SOURCE_DIR}/runtime/Cpp/runtime/src)

set(GRAMMARS Arithmetic PropositionalLogic LinearTemporalLogic PastTimeLinearTemporalLogic MetricTemporalLogic SignalTemporalLogic)

foreach(GRAMMAR IN LISTS GRAMMARS)
    set(LEXER ${GRAMMAR}Lexer)
    set(PARSER ${GRAMMAR}Parser)

    antlr_target(${LEXER} ${LEXER}.g4
        LEXER
        PACKAGE forek)

    antlr_target(${PARSER} ${PARSER}.g4
        PARSER
        VISITOR
        PACKAGE forek
        DEPENDS_ANTLR ${LEXER}
        COMPILE_FLAGS -lib ${ANTLR_${LEXER}_OUTPUT_DIR} -DtokenVocab=${LEXER})

    target_sources(forek_grammars
        PRIVATE ${ANTLR_${LEXER}_CXX_OUTPUTS} ${ANTLR_${PARSER}_CXX_OUTPUTS})

    target_include_directories(forek_grammars 
        PUBLIC ${ANTLR_${LEXER}_OUTPUT_DIR} ${ANTLR_${PARSER}_OUTPUT_DIR})
endforeach()

target_compile_features(forek_grammars PUBLIC cxx_std_17)

target_link_libraries(forek_grammars PRIVATE antlr4_static)
