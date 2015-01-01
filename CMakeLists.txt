cmake_minimum_required (VERSION 2.6)
project (Reludp)

include_directories(include)

add_library(menu src/menu.c)
add_library(db_functions src/db_functions.c)

add_executable(vgp src/vgp.c)
target_link_libraries(vgp menu)
target_link_libraries(menu db_functions)
