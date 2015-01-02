#include "tests/test_data.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char places[5][LEN_MESTO+1] = {
    "Liman",
    "Detel",
    "NNaselje",
    "Salajka",
    "SlBara",
};


void init_test_data()
{
    srand( time(NULL) );
    int i;
    for(i = 0; i < 5; i++)
    {
        sprintf(sample_entries[i].e_br, "%09d", rand());

        sprintf(sample_entries[i].reg_br, "%03d", i);
        time(&sample_entries[i].epoch);

        strcpy(sample_entries[i].mesto, places[i]);

        sample_entries[i].boravak = rand() % 1000000;
    }
}
