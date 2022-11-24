cmake_minimum_required(VERSION 3.5)

project(HW3_Hashmap LANGUAGES C)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(HW3_Hashmap main.c customhash.c customhash.h)
