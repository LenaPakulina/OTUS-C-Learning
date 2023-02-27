cmake_minimum_required(VERSION 3.5)

project(library LANGUAGES C)

set(This sLogger)

add_library(${This} sLogger.h sLogger.c)
target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
