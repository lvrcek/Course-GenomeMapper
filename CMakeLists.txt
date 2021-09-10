cmake_minimum_required(VERSION 3.14)

project(ivory_mapper VERSION 0.2.0
                     LANGUAGES CXX
                     DESCRIPTION "A simple ivory_mapper for long genomic reads")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
    bioparser
    GIT_REPOSITORY https://github.com/rvaser/bioparser
    GIT_TAG 3.0.13
)
FetchContent_MakeAvailable(bioparser)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(include)

add_executable(ivory_mapper src/main.cpp)

configure_file(include/ivory_config.hpp.in ivory_config.hpp)

target_link_libraries(ivory_mapper
    bioparser::bioparser
    ivory_alignment_engine
)

target_include_directories(ivory_mapper PUBLIC
    "${PROJECT_BINARY_DIR}"
    "include"
)

# Testing
enable_testing()

add_executable(ivory_mapper_test test/ivory_mapper_test.cpp)

target_link_libraries(ivory_mapper_test
    gtest_main
    bioparser::bioparser
    ivory_alignment_engine
)

target_include_directories(ivory_mapper_test PUBLIC
    "include"
)

include(GoogleTest)
gtest_discover_tests(ivory_mapper_test)
