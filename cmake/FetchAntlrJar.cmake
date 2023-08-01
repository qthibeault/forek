cmake_minimum_required(VERSION 3.11)

function(FetchAntlrJar)
    cmake_parse_arguments(ANTLR_JAR "" "VERSION;" "")

    include(FetchContent)

    if(NOT ANTLR_JAR_VERSION)
        set(ANTLR_JAR_VERSION 4.13.0)
    endif()

    set(ANTLR_JAR_FILENAME antlr-${ANTLR_JAR_VERSION}-complete.jar)

    FetchContent_Declare(
        antlr4jar
        URL https://www.antlr.org/download/${ANTLR_JAR_FILENAME}
        DOWNLOAD_NO_EXTRACT TRUE)

    FetchContent_MakeAvailable(antlr4jar)

    set(ANTLR_EXECUTABLE ${antlr4jar_SOURCE_DIR}/${ANTLR_JAR_FILENAME} PARENT_SCOPE)
endfunction()
