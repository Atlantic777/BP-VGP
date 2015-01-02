#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "tests/db_functions.h"
#include "tests/seq_file.h"

static FILE *f = NULL;

int main()
{
    CU_pSuite pSuite = NULL;

    CU_initialize_registry();

    pSuite = CU_add_suite("DB Functions", init_db_functions, clean_db_functions);

    CU_add_test(pSuite, "Write 1 entry", test_rw_one_entry);
    CU_add_test(pSuite, "Write 5 entries", test_rw_five_entries);

    CU_pSuite seqSuite = NULL;

    seqSuite = CU_add_suite("Sequential file maniuplation", init_seq_functions, clean_seq_functions );
    CU_add_test(seqSuite, "Sort entries from serial file", sort_entries );
    CU_add_test(seqSuite, "Initialize sequential file", init_seq_file );
    CU_add_test(seqSuite, "Store data to sequential file", store_to_seq_file );


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
