cmake_minimum_required(VERSION 3.10)

project(GenomeMapper VERSION 0.1.0
                     LANGUAGES CXX
                     DESCRIPTION "A simple mapper for long genomic reads")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(mapper src/main.cpp)