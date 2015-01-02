#include "tests/db_functions.h"
#include <stdio.h>
#include "vgp.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "db_functions.h"
#include <CUnit/CUnit.h>
#include "utils.h"
#include "tests/test_data.h"

FILE *f = NULL;
vgp_parkiranje sample_entries[5];

int cmp_vgp_entries(vgp_parkiranje *given, vgp_parkiranje *expected)
{
    CU_ASSERT_STRING_EQUAL(given->e_br, expected->e_br);
    CU_ASSERT_STRING_EQUAL(given->reg_br, expected->reg_br);
    CU_ASSERT_EQUAL( given->epoch, expected->epoch );
    CU_ASSERT_STRING_EQUAL( given->mesto, expected->mesto );
    CU_ASSERT_EQUAL( given->boravak, expected->boravak );
}


int init_db_functions()
{
    srand( time(NULL) );
    f = fopen("/tmp/db_functions_test.db", "w+");
    init_test_data();


    return 0;
}

int clean_db_functions()
{
    fclose(f);
    return 0;
}

void test_rw_one_entry()
{
    vgp_parkiranje *out = &sample_entries[0];
    db_store_vgp(f, out);

    vgp_parkiranje in;
    fseek(f, 0, SEEK_SET);
    db_read_vgp(f, &in);

    cmp_vgp_entries( &in, out );
}


void test_rw_five_entries()
{
    int i;

    fseek(f, 0, SEEK_SET);
    for(i = 0; i < 5; i++)
    {
        db_store_vgp(f, &sample_entries[i]);
    }

    vgp_parkiranje in_entries[5];

    fseek(f, 0, SEEK_SET);
    for(i = 0; i < 5; i++)
    {
        db_read_vgp(f, &in_entries[i]);
        //print_vgp_entry( &in_entries[i] );
    }

    for(i = 0; i < 5; i++)
    {
        cmp_vgp_entries( &in_entries[i], &sample_entries[i] );
    }
}
