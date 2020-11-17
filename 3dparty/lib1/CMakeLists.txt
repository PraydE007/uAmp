cmake_minimum_required(VERSION 3.18.2)

project("taglib")

set(TAGLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options(-std=c++11)

# message(STATUS "TAGLIB DIRECTORY !!!!!!!!!  ${TAGLIB_DIR}  !!!!!!!!!!!!!!!")

file(GLOB CPPS
    "*.h"
    "*.cpp"
)

add_library("taglib" ${CPPS})

include_directories(".")
include_directories("frames")
