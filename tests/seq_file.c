#include "tests/seq_file.h"
#include "tests/test_data.h"
#include <CUnit/CUnit.h>
#include <stdio.h>
#include "db_functions.h"
#include "utils.h"
#include <stdlib.h>

int init_seq_functions()
{
    init_test_data();

    return 0;
}

int clean_seq_functions()
{
    return 0;
}

void sort_entries()
{
    vgp_sort(sample_entries, 5);

    int i;
    for(i = 0; i < 4; i++)
    {
        if( atoi(sample_entries[i].e_br) > atoi(sample_entries[i+1].e_br) )
        {
            CU_FAIL("Keys not ascending");
            break;
        }
    }
}

void init_seq_file()
{
    CU_FAIL("Finish the test...");
}

void store_to_seq_file()
{
    CU_FAIL("Finish the test...");
}
