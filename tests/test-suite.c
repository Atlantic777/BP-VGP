#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "tests/db_functions.h"

static FILE *f = NULL;

int main()
{
    CU_pSuite pSuite = NULL;

    CU_initialize_registry();

    pSuite = CU_add_suite("DB Functions", init_db_functions, clean_db_functions);

    CU_add_test(pSuite, "Write 1 entry ", test_rw_one_entry);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
