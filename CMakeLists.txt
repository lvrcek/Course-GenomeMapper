cmake_minimum_required(VERSION 3.14)

project(ivory_mapper VERSION 0.1.0
                     LANGUAGES CXX
                     DESCRIPTION "A simple ivory_mapper for long genomic reads")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include(FetchContent)

find_package(bioparser 3.0.13 QUIET)
if (NOT bioparser_FOUND)
    FetchContent_Declare(
        bioparser
        GIT_REPOSITORY https://github.com/rvaser/bioparser
        GIT_TAG 3.0.13
    )

    FetchContent_GetProperties(bioparser)
    if (NOT bioparser_POPULATED)
    FetchContent_Populate(bioparser)
    add_subdirectory(${bioparser_SOURCE_DIR} ${bioparser_BINARY_DIR})
    endif()
endif()

find_package(GTest 1.10.0 QUIET)
if (NOT GTest_FOUND)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
    )
    FetchContent_MakeAvailable(googletest)
    add_library(GTest::Main ALIAS gtest_main)
endif()

# add_library(ivory_alignment_engine
#     include/alignment.cpp
# )

add_executable(ivory_mapper src/main.cpp)

target_link_libraries(ivory_mapper
    bioparser::bioparser
)

enable_testing()

add_executable(ivory_mapper_test test/ivory_mapper_test.cpp)
target_link_libraries(ivory_mapper_test GTest::Main)
target_link_libraries(ivory_mapper_test bioparser::bioparser)

include(GoogleTest)
gtest_discover_tests(ivory_mapper_test)