cmake_minimum_required (VERSION 2.6)
project (Reludp)

find_library(CUNIT_LIB cunit)

include_directories(include)

add_library(utils src/utils.c)

add_library(cli_functions src/cli_functions.c)
target_link_libraries(cli_functions utils)

add_library(db_file src/db_file.c)

add_library(menu src/menu.c)
target_link_libraries(menu cli_functions)

add_library(db_functions src/db_functions.c)

add_executable(vgp src/vgp.c)
target_link_libraries(vgp menu)
target_link_libraries(vgp db_file)

add_library(test_data tests/test_data.c)

add_library(t_db_functions tests/db_functions.c)
target_link_libraries(t_db_functions  test_data db_functions)

add_library(t_seq_file tests/seq_file.c)
target_link_libraries(t_seq_file utils)

add_executable(test-suite tests/test-suite.c)
target_link_libraries(test-suite cunit db_functions t_db_functions utils t_seq_file)
