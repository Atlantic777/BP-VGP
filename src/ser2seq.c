#include "ser2seq.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "db_functions.h"

int ser2seq(char *filename_serial, char *filename_sequential)
{
    FILE *f_seq = fopen(filename_sequential, "w");
    int ser_count = 0;
    vgp_parkiranje *vgp_arr;

    ser_count = load_ser(filename_serial, &vgp_arr);
    vgp_sort( vgp_arr, ser_count );

    fwrite(&ser_count, sizeof(ser_count), 1, f_seq);

    int i;
    for(i = 0; i < ser_count; i++)
    {
        db_store_vgp(f_seq, &vgp_arr[i]);
    }

    free(vgp_arr);
    fclose(f_seq);

    return ser_count;
}
