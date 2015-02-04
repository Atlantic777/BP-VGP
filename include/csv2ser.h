#ifndef CSV2SER_H
#define CSV2SER_H

#include "db_file.h"

#define CSV_LINE_LEN 100
int csv2ser(char *filename_csv, db_file *dbf);

#endif
