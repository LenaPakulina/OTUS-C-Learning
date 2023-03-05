cmake_minimum_required(VERSION 3.5)

project(HW7_ASM LANGUAGES C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(HW7_ASM main.c)
