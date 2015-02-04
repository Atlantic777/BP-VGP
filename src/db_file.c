#include "db_file.h"
#include <stdlib.h>
#include <string.h>

void init_db_file(db_file *dbf)
{
    dbf = malloc(sizeof(db_file));
    memset(dbf, 0, sizeof(db_file) );
}

void open_db_files(db_file *dbf, char *path, char *filename)
{
    char fname[100];
    char tmp[100];

    strcpy(fname, path);
    strcat(fname, filename);

    puts("START");

    strcpy(tmp, fname);
    strcat(tmp, SER_F_SUFFIX);
    puts(tmp);
    dbf->f_ser = fopen(tmp, "a+");

    strcpy(tmp, fname);
    strcat(tmp, SEQ_F_SUFFIX);
    puts(tmp);
    dbf->f_seq = fopen(tmp, "a+");

    strcpy(tmp, fname);
    strcat(tmp, MAIN_F_SUFFIX);
    puts(tmp);
    dbf->f_main = fopen(tmp, "a+");

    strcpy(tmp, fname);
    strcat(tmp, IDX_F_SUFFIX);
    puts(tmp);
    dbf->f_idx = fopen(tmp, "a+");

    strcpy(tmp, fname);
    strcat(tmp, OVF_F_SUFFIX);
    puts(tmp);
    dbf->f_ovf = fopen(tmp, "a+");

    strcpy(dbf->path, path);
    strcpy(dbf->f_prefix, filename);
}

void close_db_files(db_file *dbf)
{
    fclose(dbf->f_ser);
    fclose(dbf->f_seq);
    fclose(dbf->f_main);
    fclose(dbf->f_idx);
    fclose(dbf->f_ovf);
    dbf->f_prefix[0] = 0;
}
