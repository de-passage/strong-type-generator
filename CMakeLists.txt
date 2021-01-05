cmake_minimum_required(VERSION 3.0.0)
project(strong-type-generator VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

set(DEMO_FILES 
    ${SOURCE_DIR}/demo.cpp)

add_executable(demo ${DEMO_FILES})
target_include_directories(demo PRIVATE ${HEADER_DIR})