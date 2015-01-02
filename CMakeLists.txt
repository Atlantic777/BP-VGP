cmake_minimum_required (VERSION 2.6)
project (Reludp)

include_directories(include)

add_library(utils src/utils.c)

add_library(db_functions src/db_functions.c)
target_link_libraries(db_functions utils)

add_library(db_file src/db_file.c)

add_library(menu src/menu.c)
target_link_libraries(menu db_functions)

add_executable(vgp src/vgp.c)
target_link_libraries(vgp menu)
target_link_libraries(vgp db_file)
