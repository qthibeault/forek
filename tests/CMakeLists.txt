include(FetchContent)
include(CTest)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

function(make_test LOGIC_TYPE)
    set(TARGET_NAME test_${LOGIC_TYPE})

    add_executable(${TARGET_NAME}
        ${TARGET_NAME}.cxx)

    target_link_libraries(${TARGET_NAME} PRIVATE
        forek::forek
        Catch2::Catch2WithMain
        fmt::fmt-header-only)

    catch_discover_tests(${TARGET_NAME})
endfunction()

make_test(pl)
make_test(ltl)
make_test(mtl)
make_test(stl)
