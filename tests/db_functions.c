#include "tests/db_functions.h"
#include <stdio.h>
#include "vgp.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "db_functions.h"
#include <CUnit/CUnit.h>
#include "utils.h"

FILE *f = NULL;
vgp_parkiranje sample_entries[5];


int init_db_functions()
{
    srand( time(NULL) );
    f = fopen("/tmp/db_functions_test.db", "w+");

    char e_br_sample[30] = "123456780";
    char places[5][LEN_MESTO+1] = {
            "Liman",
            "Detel",
            "NNaselje",
            "Salajka",
            "SlBara",
            };

    int i;
    for(i = 0; i < 5; i++)
    {
        e_br_sample[8]++;
        strcpy(sample_entries[i].e_br, e_br_sample);

        sprintf(sample_entries[i].reg_br, "%03d", i);
        time(&sample_entries[i].epoch);

        strcpy(sample_entries[i].mesto, places[i]);

        sample_entries[i].boravak = rand() % 1000000;
    }

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

    CU_ASSERT_STRING_EQUAL(out->e_br, in.e_br);
    CU_ASSERT_STRING_EQUAL(out->reg_br, in.reg_br);
    CU_ASSERT_EQUAL(out->epoch, in.epoch);
}


void teset_read_five_entries()
{
    int i;
    for(i = 0; i < 5; i++)
    {
        db_store_vgp(f, &sample_entries[i]);
    }

    for(i = 0; i < 5; i++)
    {

    }
}
