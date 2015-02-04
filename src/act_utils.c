#include "act_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vgp.h"
#include "db_functions.h"
#include "bst_utils.h"

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
    rewind(dbf->f_main);
    rewind(dbf->f_idx);
    rewind(dbf->f_ovf);

    printf("searching block for %s\n", key);
    int offset = find_block_for_key(dbf->f_idx, key);

    main_block block;
    overflow_block ovf_block;

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

    if(i == 5 && block.n_overflows == 0 )
    {
        return 2;
    }
    else
    {
        int last_ovf_offset;
        load_ovf_block( dbf->f_ovf, block.first_overflow_offset, (struct overflow_block*)&ovf_block);
        last_ovf_offset = block.first_overflow_offset;
        printf("fetching first overflow, key is %s\n", ovf_block.entry.e_br);

        while( 1 )
        {
            printf("Overflow key: %s\n", ovf_block.entry.e_br);

            if( strcmp( key, ovf_block.entry.e_br ) == 0 )
            {
                if( result != NULL )
                    memcpy( result, &ovf_block.entry, sizeof( vgp_parkiranje ) );

                return 1;
            }
            else
            {
                if( ovf_block.next_entry_offset != -1)
                {
                    puts("fetching next overflow");
                    last_ovf_offset = ovf_block.next_entry_offset;
                    load_ovf_block( dbf->f_ovf, ovf_block.next_entry_offset, (struct overflow_block*)&ovf_block );
                }
                else
                {
                    return last_ovf_offset;
                }

            }
        }
        return 10*last_ovf_offset;
    }

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
   else if(entry_offset == 2)
   {
       puts("doing first overflow");

       int last_addr;
       last_addr = fseek(dbf->f_ovf, 0, SEEK_END);
       last_addr /= sizeof( overflow_block );

       overflow_block ovf;
       memcpy(&ovf.entry, entry, sizeof( vgp_parkiranje ) );
       ovf.next_entry_offset = -1;

       main_block m_block;
       load_main_block( dbf->f_main, block_offset, &m_block );
       m_block.n_overflows++;
       m_block.first_overflow_offset = last_addr;
       fseek( dbf->f_main, block_offset*sizeof(main_block), SEEK_SET);
       fwrite( &m_block, sizeof(main_block), 1, dbf->f_main);

       fwrite( &ovf, sizeof(overflow_block), 1, dbf->f_ovf );
       printf("%d\n", last_addr);

       fflush(dbf->f_main);
       fflush(dbf->f_ovf);
   }
   else
   {
       puts("doing second overflow");

       int last_addr;
       fseek(dbf->f_ovf, 0, SEEK_END);
       last_addr = ftell(dbf->f_ovf);
       last_addr /= sizeof( overflow_block );
       printf("I would fetch ovf %d and store in %d.\n", entry_offset/10, last_addr);

       overflow_block ovf;

       // store new entry
       memcpy(&ovf.entry, entry, sizeof( vgp_parkiranje ) );
       ovf.next_entry_offset = -1;
       fwrite( &ovf, sizeof(overflow_block), 1, dbf->f_ovf );

       // get prev entry
       load_ovf_block(dbf->f_ovf, entry_offset/10, (struct overflow_block*)&ovf);
       ovf.next_entry_offset = last_addr;
       fseek(dbf->f_ovf, (entry_offset/10)*sizeof(overflow_block), SEEK_SET);
       fwrite( &ovf, sizeof(overflow_block), 1, dbf->f_ovf );

       main_block m_block;
       load_main_block( dbf->f_main, block_offset, &m_block );
       m_block.n_overflows++;
       fseek( dbf->f_main, block_offset*sizeof(main_block), SEEK_SET);
       fwrite( &m_block, sizeof(main_block), 1, dbf->f_main);

       fflush(dbf->f_main);
       fflush(dbf->f_ovf);

       // get last overflow
       // move one next
       // store
   }
}

int load_ovf_block(FILE *f_ovf, int n, struct overflow_block *result)
{
    rewind(f_ovf);

    fseek(f_ovf, n*sizeof( overflow_block ), SEEK_SET);

    return fread( result, sizeof(overflow_block), 1, f_ovf);
}
