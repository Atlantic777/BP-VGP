#include "csv2ser.h"
#include "vgp.h"
#include <stdio.h>
#include <stdlib.h>
#include "db_functions.h"
#include "utils.h"

int csv2ser(char *filename_csv, char *filename_serial)
{
    FILE *f_csv = fopen(filename_csv, "r");
    FILE *f_ser = fopen(filename_serial, "w");

    if( f_csv == NULL )
        puts("something went wrong with opening the CSV file");

    int count = 0;
    char *current_line[CSV_LINE_LEN];
    vgp_parkiranje vgp_e;

    ssize_t len = CSV_LINE_LEN;

    fseek(f_ser, sizeof(int), SEEK_SET);

    while( getline( (char**)&current_line, &len, f_csv) != -1 )
    {
        sscanf((const char*)*current_line, "%s | %s | %d | %s | %ld",
                    vgp_e.e_br, vgp_e.reg_br, (int*)(&vgp_e.epoch), vgp_e.mesto, &vgp_e.boravak);

        db_store_vgp(f_ser, &vgp_e);

        count++;
    }

    rewind(f_ser);
    fwrite(&count, sizeof(count), 1, f_ser);

    fclose(f_csv);
    fclose(f_ser);

    return count;
}
