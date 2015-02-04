#include "ser2seq.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "db_functions.h"

int ser2seq(db_file *dbf)
{
    //FILE *f_seq = dbf->f_seq;
    int ser_count = 0;
    vgp_parkiranje *vgp_arr;

    ser_count = load_ser(dbf, &vgp_arr);
    vgp_sort( vgp_arr, ser_count );

    fwrite(&ser_count, sizeof(ser_count), 1, dbf->f_seq);

    int i;
    for(i = 0; i < ser_count; i++)
    {
        db_store_vgp(dbf->f_seq, &vgp_arr[i]);
    }

    free(vgp_arr);
    fflush(dbf->f_seq);

    return ser_count;
}
