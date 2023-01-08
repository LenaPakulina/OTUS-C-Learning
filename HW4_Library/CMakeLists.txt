cmake_minimum_required(VERSION 3.5)

project(HW4_Library LANGUAGES C)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(HW4_Library src/main.c src/parser_json.c src/getter.c)
target_link_libraries(HW4_Library curl)
target_link_libraries(HW4_Library json-c)

