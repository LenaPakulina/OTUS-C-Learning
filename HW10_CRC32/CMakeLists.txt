cmake_minimum_required(VERSION 3.5)

project(HW10_CRC32 LANGUAGES C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(HW10_CRC32 main.c crc32_utils.h crc32_utils.c)
