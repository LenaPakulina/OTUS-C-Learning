cmake_minimum_required(VERSION 3.5)

project(HW17_FinishProject_Taxi LANGUAGES C)
set(This HW17_FinishProject_Taxi)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

find_package(PkgConfig REQUIRED)
pkg_check_modules(APPINDICATOR REQUIRED appindicator3-0.1)
include_directories(${APPINDICATOR_INCLUDE_DIRS})

set(ICONS ./icons/)
install(DIRECTORY ${ICONS} DESTINATION $ENV{HOME}/.config/runcat/icons)

add_executable(HW17_FinishProject_Taxi src/main.c src/getter.h src/getter.c src/parser_json.h src/parser_json.c src/price.h src/db_integration.c src/db_integration.h src/globals.h)
target_link_libraries(HW17_FinishProject_Taxi curl)
target_link_libraries(HW17_FinishProject_Taxi json-c)
target_link_libraries(HW17_FinishProject_Taxi ${APPINDICATOR_LIBRARIES})

execute_process(COMMAND pg_config --pkgincludedir OUTPUT_VARIABLE PG_INCLUDE_DIR)
target_include_directories(${This} PUBLIC ${PG_INCLUDE_DIR} )
target_link_libraries(${This} pq)
target_compile_options(${This} PUBLIC -Wall -Wextra -Wpedantic -std=gnu11)
