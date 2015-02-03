#include "act_utils.h"
#include "bst_utils.h"
#include <string.h>

int build_index_file(char *file_prefix, struct index_entry *keys, int count)
{
    char filename_index[256];
    int i = 0;

    strcpy(filename_index, file_prefix);
    strcat(filename_index, "_idx.db");

    FILE *f_idx = fopen(filename_index, "w");

    fclose(f_idx);
    return 0;
}
