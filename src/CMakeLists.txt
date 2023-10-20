include(GNUInstallDirs)

# Ensure code is compiled with PIC flag
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Import grammar target
add_subdirectory(grammars)

# Ensure Git is installed
find_package(Git REQUIRED)

# Enable FetchContent module to download dependencies
include(FetchContent)

# Download fmt library
FetchContent_Declare(fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 10.1.0)

# Until CMake 3.28 is released, this is the way to exclude imported project targets
FetchContent_GetProperties(fmtlib)

if(NOT fmtlib_POPULATED)
    FetchContent_Populate(fmtlib)
    add_subdirectory(${fmtlib_SOURCE_DIR} ${fmtlib_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_library(forek interval.cxx common.cxx algebra.cxx pl.cxx ltl.cxx mtl.cxx stl.cxx stpl.cxx tptl.cxx tqtl.cxx)

target_include_directories(forek
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/forek>)

target_link_libraries(forek
    PRIVATE $<BUILD_INTERFACE:forek_grammars> $<BUILD_INTERFACE:fmt::fmt-header-only>)

target_compile_features(forek PUBLIC cxx_std_17)

# Set compile options only for source files of forek library to avoid errors in ANTLR generated code
if (MSVC)
    # warning level 4
    set_source_files_properties($<TARGET_PROPERTY:forek,SOURCES>
        COMPILE_OPTIONS /Wall /WX)
else()
    # additional warnings
    set_source_files_properties($<TARGET_PROPERTY:forek,SOURCES>
        COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror)
endif()

add_library(forek::forek ALIAS forek)

install(TARGETS forek EXPORT forek ARCHIVE)
install(EXPORT forek DESTINATION lib/forek)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/forek TYPE INCLUDE)
