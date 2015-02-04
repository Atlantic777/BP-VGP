#include "csv2ser.h"
#include "vgp.h"
#include <stdio.h>
#include <stdlib.h>
#include "db_functions.h"
#include "utils.h"

int csv2ser(char *filename_csv, db_file *dbf)
{
    FILE *f_csv = fopen(filename_csv, "r");
    FILE *f_ser = dbf->f_ser;

    if( f_csv == NULL )
        puts("something went wrong with opening the CSV file");


    int count = 0;
    char *current_line = malloc( CSV_LINE_LEN );
    vgp_parkiranje vgp_e;

    ssize_t len = CSV_LINE_LEN;

    fwrite( &count, sizeof(count), 1, f_ser );

    while( getline( &current_line, &len, f_csv) != -1 )
    {
        sscanf((const char*)current_line, "%s | %s | %d | %s | %ld",
                    vgp_e.e_br, vgp_e.reg_br, (int*)(&vgp_e.epoch), vgp_e.mesto, &vgp_e.boravak);

        db_store_vgp(f_ser, &vgp_e);

        count++;
    }

    free(current_line);

    rewind(f_ser);
    fwrite(&count, sizeof(count), 1, f_ser);

    fclose(f_csv);
    fflush(f_ser);

    return count;
}
