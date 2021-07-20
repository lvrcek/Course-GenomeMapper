cmake_minimum_required(VERSION 3.11)

project(GenomeMapper VERSION 0.1.0
                     LANGUAGES CXX
                     DESCRIPTION "A simple mapper for long genomic reads")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
    add_subdirectory(
        ${bioparser_SOURCE_DIR}
        ${bioparser_BINARY_DIR}
        EXCLUDE_FROM_ALL    
    )
    endif()
endif()

add_executable(mapper src/main.cpp)

target_link_libraries(mapper bioparser::bioparser)