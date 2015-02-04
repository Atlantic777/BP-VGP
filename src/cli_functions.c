#include "cli_functions.h"
#include <stdio.h>
#include <string.h>
#include "db_file.h"
#include "vgp.h"
#include "utils.h"
#include "bst_utils.h"
#include "act_utils.h"
#include <stdlib.h>
#include "init_db.h"
#include "db_functions.h"
#include "ser2seq.h"
#include "seq2act.h"

#define ROOT_DIR "/tmp/"

db_file dbf;

int cli_exit_program()
{
    puts(dbf.f_prefix);
    close_db_files(&dbf);
    return 0;
}

int cli_create_new_file()
{
    char filename[30];

    puts("Kreiranje nove datoteke");
    puts("=======================");
    printf("Naziv datoteke: "); scanf("%s", filename);

    open_db_files( &dbf, ROOT_DIR, filename );
    close_db_files( &dbf );

    return 0;
}

int do_next_entry()
{
    int  resp, ok = 0, i;
    char ans;
    char possible[4] = { 'N', 'Y', 'n', 'y' };

    do {
        printf("Sledeci unos? [Y/n]: ");
        resp = scanf("%c", &ans);

        if(resp == 0)
            return 1;

        if(resp != 1)
            break;

        for(i = 0; i < 4; i++)
        {
            if(ans == possible[i])
            {
                ok = i;
                break;
            }
        }
    } while( !ok );

    return ans%2;
}

int cli_create_from_data()
{
    puts("Kreiranje baze podataka");
    puts("=======================");

    if( strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    create_from_data( &dbf, "data.csv");

    return 0;
}

int cli_choose_file()
{
    char filename[30];

    puts("Otvaranje datoteke");
    puts("==================");
    printf("Naziv datoteke: "); scanf("%s", filename);

    open_db_files( &dbf, ROOT_DIR, filename );


    return 0;
}

int cli_show_filename()
{
    if( strlen(dbf.f_prefix) == 0)
        puts("Nije otvorena ni jedna datoteka");
    else
        puts(dbf.f_prefix);

    return 0;
}

int cli_create_serial_file()
{
    if( strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }
    else
    {
        vgp_parkiranje tmp;
        int ok = 1;
        int resp;

        int count = 0;
        fwrite( &count, sizeof(count), 1, dbf.f_ser);

        do {
            tmp = create_new_vgp_entry();
            db_store_vgp( dbf.f_ser, &tmp );
            count++;
        } while( do_next_entry() );

        rewind(dbf.f_ser);
        fwrite( &count, sizeof(count), 1, dbf.f_ser);
    }
    fflush(dbf.f_ser);
}

int cli_print_idx()
{
    rewind(dbf.f_idx);
    int i = 0;
    stored_index_block *current_block = malloc( sizeof(stored_index_block) );

    for(i = 0; i < 31; i++)
    {
        load_idx_block(dbf.f_idx, i, (struct stored_index_block*)current_block);
        printf("[%2d] - (%4s - %3d)\t-\t(%4s - %3d)\n", i, current_block->entries[0].key, current_block->entries[0].block_addr,
                                                         current_block->entries[1].key, current_block->entries[1].block_addr);
    }

    free(current_block);
}

int cli_print_main()
{
    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    main_block current_block;

    int i = 0;
    int j;

    while( load_main_block( dbf.f_main, i, &current_block) )
    {
        printf("%3d *** [%3d] *  ", i, current_block.n_overflows);

        for(j = 0; j < 5; j++)
        {
            printf("%10s", current_block.entries[j].e_br);
        }

        printf("\n");
        i++;
    }
}

int cli_find_key()
{
    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    char key[10];

    printf("Key to find: "); scanf("%s", key);

    int r = find_block_for_key(dbf.f_idx, key);
    printf("Matching block %d\n", r);
    return 0;
}

int cli_find_entry()
{
    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    main_block block;

    char key[10];
    printf("Key to find: "); scanf("%s", key);

    vgp_parkiranje res;
    int resp = find_entry(&dbf, key, &res);

    if( resp == 1)
        print_vgp_entry( &res );
    else if(resp == 0)
        puts("Nema ga!");
}

int cli_store_entry()
{
    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    vgp_parkiranje p = create_new_vgp_entry();

    store_entry( &dbf, &p );

    return 0;
}

int cli_create_seq()
{
    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    ser2seq(&dbf);

    return 0;
}

int cli_create_act()
{
    seq2act(&dbf);
    return 0;
}

int cli_reorganize()
{
    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    //close_db_files( &dbf );
    //open_db_files( &dbf, dbf.path, dbf.f_prefix );

    // check if should reorganize
    rewind(dbf.f_main);
    int count;
    int should = 0;

    main_block current_block;

    int i = 0;
    int j;

    while( load_main_block( dbf.f_main, i, &current_block) )
    {
        //printf("Block: %2d - [%2d]\n", i, current_block.n_overflows);

        if( current_block.n_overflows >= 3 )
        {
            should = 1;
            break;
        }
        i++;
    }

    should = 1;
    int next = -1;

    if(should == 0)
    {
        puts("Shouldn't reorganize");
        return -1;
    }

    db_file t_dbf;
    open_db_files(&t_dbf, "/tmp/", "reorg");

    overflow_block ovf;

    puts("Starting reorganize");

    count = 0;
    fwrite(&count, sizeof(count), 1, t_dbf.f_ser);

    i = 0;
    while( load_main_block( dbf.f_main, i, &current_block) )
    {
        // extract normal entries
        for(j = 0; j < 5; j++)
        {
            if( strlen( current_block.entries[j].e_br) == 0)
                break;
            else
            {
                puts(current_block.entries[j].e_br);
                db_store_vgp( t_dbf.f_ser, &current_block.entries[j] );
                count++;
            }

        }

        // extract overflows
        if( current_block.n_overflows > 0)
        {
            puts("Got overflow!");
            next = current_block.first_overflow_offset;

            while(next != -1)
            {
                puts("doing overflow");
                load_ovf_block( dbf.f_ovf, next, (struct overflow_block*)&ovf);
                db_store_vgp( t_dbf.f_ser, &ovf.entry );
                next = ovf.next_entry_offset;
                count++;
            }
        }
        i++;
    }

    rewind(t_dbf.f_ser);
    fwrite(&count, sizeof(count), 1, t_dbf.f_ser);

    ser2seq(&t_dbf);
    seq2act(&t_dbf);

    close_db_files(&t_dbf);

    return 0;
}
