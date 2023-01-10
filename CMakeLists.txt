cmake_minimum_required(VERSION 3.22)

get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${TARGET})
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(COMPILE_FLAGS -pipe -Wall -Wextra -Weffc++ -fPIC -ggdb3 -O3)
set(COMPILE_DEFS _REENTRANT _FORTIFY_SOURCE=2 _GLIBCXX_ASSERTIONS)
set(LINK_FLAGS -rdynamic)

file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE INCLUDE "include/*.h")

add_executable(${TARGET} ${SRC})
target_link_libraries(${TARGET})
#target_include_directories(${TARGET} PRIVATE)
target_compile_options(${TARGET} PRIVATE ${COMPILE_FLAGS})
target_link_options(${TARGET} PRIVATE ${LINK_FLAGS})
target_compile_definitions(${TARGET} PRIVATE ${COMPILE_DEFS})