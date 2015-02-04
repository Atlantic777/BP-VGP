#include "act_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vgp.h"
#include "db_functions.h"

void build_overrun_file(char *file_prefix)
{
    char filename[255];

    strcpy(filename, file_prefix);
    strcat(filename, "_ovf.db");
    FILE *f = fopen(filename, "w");
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
    rewind(f_idx);
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

        if( (key_val > e1_key) && (key_val <= e2_key) )
        {
            return current_idx_block->entries[1].block_addr;
        }
        else if( (key_val > e2_key) && (e2_key != 0) )
        {
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

int find_entry(db_file *dbf, char *key, vgp_parkiranje *result)
{
    printf("searching block for %s\n", key);
    int offset = find_block_for_key(dbf->f_idx, key);

    main_block block;
    puts("fetching block");
    load_main_block(dbf->f_main, offset, &block);

    int i;

    // search inside the block
    puts("starting search");
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

int store_entry(db_file *dbf, vgp_parkiranje *entry)
{
   int i;

   puts("store entry");
   int block_offset = find_block_for_key(dbf->f_idx, entry->e_br);
   int entry_offset = find_entry(dbf, entry->e_br, NULL);

   if(entry_offset < 0)
   {
       printf("Empty offset is: %d\n", -entry_offset);
       fseek(dbf->f_main, block_offset*sizeof(main_block), SEEK_SET);
       fseek(dbf->f_main, -entry_offset*sizeof(vgp_parkiranje), SEEK_CUR);

       db_store_vgp(dbf->f_main, entry);
   }
   else if(entry_offset == 1)
   {
       puts("key already there!");
   }

   // or create new in overflow area
   // store it


}
