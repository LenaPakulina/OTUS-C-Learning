cmake_minimum_required(VERSION 3.5)

project(HW15_Cube LANGUAGES C)
set(This HW15_Cube)

add_executable(HW15_Cube main.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(GL REQUIRED gl)
pkg_check_modules(GLU REQUIRED glu)
target_include_directories(${This} PUBLIC
${SDL2_INCLUDE_DIRS}
${GL_INCLUDE_DIRS}
${GLU_INCLUDE_DIRS} )
target_link_libraries(${This} PUBLIC ${SDL2_LIBRARIES} ${GLU_LIBRARIES} ${GL_LIBRARIES})
target_compile_options(${This} PUBLIC -Wall -Wextra -Wpedantic -std=gnu11)
