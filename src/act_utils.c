#include "act_utils.h"
#include <stdio.h>
#include <string.h>

void build_overrun_file(char *file_prefix)
{
    char filename[255];

    strcpy(filename, file_prefix);
    strcat(filename, "_ovf.db");
    FILE *f = fopen(filename, "w");

    fclose(f);
}

int load_idx_block(FILE *f_idx, int n, struct stored_index_block *block)
{
    if( fseek(f_idx, n*sizeof(stored_index_block), SEEK_SET) )
    {
        perror("Load idx block: ");
        return -1;
    }

    return fread( block, sizeof(stored_index_block), 1, f_idx);
}

int load_main_block(FILE *f_main, int n, main_block *block)
{
    if( fseek(f_main, n*sizeof(main_block), SEEK_SET) )
    {
        perror("Load idx block: ");
        return -1;
    }

    return fread( block, sizeof(main_block), 1, f_main);
}
