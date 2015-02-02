cmake_minimum_required (VERSION 2.6)
project (Reludp)

find_library(CUNIT_LIB cunit)

include_directories(include)

add_library(utils src/utils.c)
add_library(init_db src/init_db.c)
add_library(cli_functions src/cli_functions.c)
add_library(db_file src/db_file.c)
add_library(menu src/menu.c)
add_library(db_functions src/db_functions.c)
add_library(test_data tests/test_data.c)
add_library(bst_utils src/bst_utils.c)
add_library(csv2ser src/csv2ser.c)
add_library(ser2seq src/ser2seq.c)

# test libs
add_library(t_db_functions tests/db_functions.c)
add_library(t_seq_file tests/seq_file.c)
add_library(t_act_file tests/active_file.c)

add_executable(vgp src/vgp.c)
add_executable(test-suite tests/test-suite.c)

target_link_libraries(init_db db_functions bst_utils csv2ser ser2seq m)
target_link_libraries(cli_functions utils init_db)
target_link_libraries(menu cli_functions)
target_link_libraries(vgp menu db_file init_db)
target_link_libraries(bst_utils db_file)
target_link_libraries(csv2ser db_functions)

target_link_libraries(t_db_functions  test_data db_functions)
target_link_libraries(t_seq_file utils)

target_link_libraries(test-suite cunit db_functions t_db_functions utils t_seq_file t_act_file)
