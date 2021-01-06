cmake_minimum_required(VERSION 3.17.0)
project(strong-type-generator VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

set(DEMO_FILES 
    ${SOURCE_DIR}/demo.cpp)

add_executable(demo ${DEMO_FILES})
target_include_directories(demo PRIVATE ${HEADER_DIR})

if(MSVC)
  target_compile_options(demo PRIVATE /W4 /WX)
else()
  target_compile_options(demo PRIVATE -Wall -Wextra -pedantic -Werror)
endif()