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

#define ROOT_DIR "/tmp/"

db_file dbf;

int cli_exit_program()
{
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

        do {
            tmp = create_new_vgp_entry();
            db_store_vgp( dbf.f_ser, &tmp );
        } while( do_next_entry() );
    }
}

int cli_print_idx()
{
    int i = 0;
    FILE *f = fopen("act_test_idx.db", "r");
    stored_index_block *current_block = malloc( sizeof(stored_index_block) );

    for(i = 0; i < 31; i++)
    {
        load_idx_block(f, i, (struct stored_index_block*)current_block);
        printf("[%2d] - (%4s - %3d)\t-\t(%3s - %3d)\n", i, current_block->entries[0].key, current_block->entries[0].block_addr,
                                                         current_block->entries[1].key, current_block->entries[1].block_addr);
    }

    free(current_block);
    fclose(f);
}

int cli_print_main()
{
    FILE *f = fopen("act_test_main.db", "r");

    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    main_block current_block;

    int i = 0;
    int j;

    while( load_main_block(f, i, &current_block) )
    {
        printf("%3d *** ", i);

        for(j = 0; j < 5; j++)
        {
            printf("%5s", current_block.entries[j].e_br);
        }

        printf("\n");
        i++;
    }

    fclose(f);
}

int cli_find_key()
{
    FILE *f_idx = fopen("act_test_idx.db", "r");

    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    char key[10];

    printf("Key to find: "); scanf("%s", key);

    int r = find_block_for_key(f_idx, key);
    printf("Matching block %d\n", r);

    fclose(f_idx);
    return 0;
}

int cli_find_entry()
{
    FILE *f_main = fopen("act_test_main.db", "r");

    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    main_block block;

    char key[10];
    printf("Key to find: "); scanf("%s", key);

    vgp_parkiranje res;

    if(find_entry(f_main, key, &res))
        print_vgp_entry( &res );
    else
        puts("Nema ga!");

    fclose(f_main);
}

int cli_store_entry()
{
    FILE *f_main = fopen("act_test_main.db", "r+");
    FILE *f_ovf  = fopen("act_test_ovf.db", "r+");

    if(strlen(dbf.f_prefix) == 0)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }

    vgp_parkiranje p;
    printf("Kljuc... "); scanf("%s", p.e_br );

    store_entry( dbf.f_main, f_ovf, &p );

    fclose(f_main);
    fclose(f_ovf);
    return 0;
}
