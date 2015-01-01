#include "db_file.h"
#include <stdlib.h>

void init_db_file()
{
    dbf = malloc(sizeof(db_file));
    dbf->file = NULL;
}
