cmake_minimum_required(VERSION 3.5)
project(HW11_threads LANGUAGES C)
set(This HW11_threads)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})

add_executable(${This} src/main.c src/utils.h src/utils.c src/threaddata.h src/threaddata.c src/globals.h src/hash.c src/hash.h)

add_definitions(${GLIB_CFLAGS_OTHER})
target_link_libraries(${This} PUBLIC ${GLIB_LIBRARIES} pthread pcre)
target_compile_options(${This} PUBLIC -Wall -Wextra -Wpedantic -std=gnu11)
