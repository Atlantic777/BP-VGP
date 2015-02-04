#include "act_utils.h"
#include "vgp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bst_utils.h"

#define MAIN_FILE_SUFFIX "_main.db"

typedef index_entry index_entry;

int build_main_file(db_file *dbf, vgp_parkiranje *vgp_arr, int count, struct index_entry **keys)
{

    char filename_main[256];
    int i = 0;
    int n_blocks = ceil(count/3.);

    main_block  current_main_block;

    strcpy(filename_main, dbf->f_prefix);
    strcat(filename_main, MAIN_FILE_SUFFIX );
    puts(filename_main);

    FILE *f_main = fopen(filename_main, "w+");
    dbf->f_main = f_main;
    rewind(f_main);

    *keys = malloc( sizeof(index_entry)*n_blocks);

    memset(&current_main_block, 0, sizeof(current_main_block) );

    for(i = 0; i < count; i++)
    {
        current_main_block.first_overflow_offset = -1;

        memcpy(&current_main_block.entries[i%3], &vgp_arr[i], sizeof(vgp_parkiranje));

        if( (i%3 == 2) || (i+1) == count)
        {
            fwrite( &current_main_block, sizeof(main_block), 1, f_main);

            ((index_entry*)(*keys))[i/3].block_addr = i/3;
            strcpy( ((index_entry*)(*keys))[i/3].key, vgp_arr[i].e_br );

            memset(&current_main_block.entries, 0, sizeof(vgp_parkiranje)*5);
        }
    }

    //fclose(f_main);
    fflush(f_main);

    return n_blocks;
}
