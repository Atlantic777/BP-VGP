#ifndef DB_FILE_H
#define DB_FILE_H

typedef struct {
    FILE *file;
    char path[50];
    char filename[30];
} db_file;

#endif
