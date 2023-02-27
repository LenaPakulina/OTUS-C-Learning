cmake_minimum_required(VERSION 3.5)

project(libraryTest LANGUAGES C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(libraryTest main.c)
target_link_libraries(libraryTest -lpthread)
target_link_libraries(libraryTest ${CMAKE_SOURCE_DIR}/libsLogger.a)
