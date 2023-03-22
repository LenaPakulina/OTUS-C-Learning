cmake_minimum_required(VERSION 3.5)
project(daemon_stat LANGUAGES C)
set(This daemon_stat)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})

add_executable(${This} main.c utils.h utils.c globals.h daemonize.c daemonize.h)

add_definitions(${GLIB_CFLAGS_OTHER})
target_link_libraries(${This} PUBLIC ${GLIB_LIBRARIES})
target_compile_options(${This} PUBLIC -Wall -Wextra -Wpedantic -std=gnu11)
