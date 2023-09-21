include(GNUInstallDirs)
include(FetchContent)

FetchContent_Declare(pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG v2.11.1)

FetchContent_MakeAvailable(pybind11)

pybind11_add_module(_forek MODULE
    forek/forek.cxx)

target_link_libraries(_forek PRIVATE forek::forek)

install(TARGETS _forek DESTINATION .)
