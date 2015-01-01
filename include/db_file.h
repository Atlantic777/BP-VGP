#ifndef DB_FILE_H
#define DB_FILE_H

#include <stdio.h>

typedef struct {
    FILE *file;
    char path[50];
    char filename[30];
} db_file;

void init_db_file();

db_file *dbf;

#endif
