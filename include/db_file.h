#ifndef DB_FILE_H
#define DB_FILE_H

#include <stdio.h>
#define MAIN_F_SUFFIX "_main.db"
#define IDX_F_SUFFIX  "_idx.db"
#define OVF_F_SUFFIX  "_ovf.db"
#define SER_F_SUFFIX  "_ser.db"
#define SEQ_F_SUFFIX  "_seq.db"

typedef struct {
    FILE *f_ser;
    FILE *f_seq;
    FILE *f_main;
    FILE *f_idx;
    FILE *f_ovf;
    char path[50];
    char f_prefix[30];
} db_file;

void init_db_file();
void open_db_files(db_file *dbf, char *path, char *filename);
void close_db_files(db_file *dbf);

#endif
