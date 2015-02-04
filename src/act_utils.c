#include "act_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vgp.h"

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
        perror("Load main block: ");
        return -1;
    }

    return fread( block, sizeof(main_block), 1, f_main);
}

int find_block_for_key(FILE *f_idx, char *key)
{
    int idx_offset = 0;
    int next_offset = -1;
    int key_val = atoi(key);
    int ret = 0;

    stored_index_block *current_idx_block = malloc( sizeof(stored_index_block) );
    stored_index_block *tmp_block = malloc( sizeof(stored_index_block) );

    load_idx_block(f_idx, idx_offset, (struct stored_index_block*)current_idx_block);


    while(1)
    {
        int e1_key  = atoi(current_idx_block->entries[0].key);
        int e2_key  = atoi(current_idx_block->entries[1].key);

        printf("Current keys: %3d and %3d\n", e1_key, e2_key);

        if( (key_val > e1_key) && (key_val <= e2_key) )
        {
            puts("staying here");
            return current_idx_block->entries[1].block_addr;
        }
        else if( (key_val > e2_key) && (e2_key != 0) )
        {
            puts("turning right");
            idx_offset = 2*idx_offset+2;

            ret = load_idx_block(f_idx, idx_offset, (struct stored_index_block*)tmp_block);

            if( strcmp(tmp_block->entries[0].key, "NONE") == 0)
            {
                if(e1_key < e2_key)
                {
                    return current_idx_block->entries[1].block_addr+1;
                }
                else
                {
                    return current_idx_block->entries[0].block_addr+1;
                }
            }
            else
            {
                memcpy(current_idx_block, tmp_block, sizeof( stored_index_block ) );
            }

        }
        else
        {
            // turn left, do nothing
            puts("turning left");
            idx_offset = 2*idx_offset+1;


            // if can't, use this addr
            ret = load_idx_block(f_idx, idx_offset, (struct stored_index_block*)tmp_block);

            if( strcmp(tmp_block->entries[0].key, "NONE") == 0 || ret == 0)
            {
                return current_idx_block->entries[0].block_addr;
            }
            else
            {
                memcpy(current_idx_block, tmp_block, sizeof( stored_index_block ) );
            }
        }
    }
}

int find_entry(FILE *f_main, char *key, vgp_parkiranje *result)
{
    FILE *f_idx = fopen("act_test_idx.db", "r");
    int offset = find_block_for_key(f_idx, key);

    main_block block;
    load_main_block(f_main, offset, &block);

    int i;

    // search inside the block
    for( i=0; i < MAIN_BLOCKING_FACTOR; i++)
    {
        if( strcmp( key, block.entries[i].e_br ) == 0)
        {
            if( result != NULL )
                memcpy( result, &block.entries[i], sizeof( vgp_parkiranje ) );

            return 1;
        }
        else if( strlen(block.entries[i].e_br) == 0 )
        {
            return -i;
        }
    }

    // search in overflow

    return 0;
}

int store_entry(FILE *f_main, FILE *f_ovw, vgp_parkiranje *entry)
{
   FILE *f_idx = fopen("act_test_idx.db", "r");
   int i;

   int entry_offset = find_entry(f_main, entry->e_br, NULL);

   if(entry_offset < 0)
       printf("Empty offset is: %d\n", -entry_offset);
   else if(entry_offset == 1)
       puts("key already there!");

   // or create new in overflow area
   // store it

   fclose(f_idx);

}
