cmake_minimum_required(VERSION 3.5)

project(HW17_FinishProject_Taxi LANGUAGES C)
set(This HW17_FinishProject_Taxi)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(HW17_FinishProject_Taxi main.c getter.h getter.c parser_json.h parser_json.c price.h db_integration.c db_integration.h)
target_link_libraries(HW17_FinishProject_Taxi curl)
target_link_libraries(HW17_FinishProject_Taxi json-c)

execute_process(COMMAND pg_config --pkgincludedir OUTPUT_VARIABLE PG_INCLUDE_DIR)
target_include_directories(${This} PUBLIC ${PG_INCLUDE_DIR} )
target_link_libraries(${This} pq)
target_compile_options(${This} PUBLIC -Wall -Wextra -Wpedantic -std=gnu11)
