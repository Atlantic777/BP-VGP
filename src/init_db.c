#include "init_db.h"
#include "vgp.h"
#include <stdio.h>
#include "utils.h"
#include "db_functions.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bst_utils.h"
#include "csv2ser.h"
#include "ser2seq.h"
#include "seq2act.h"

// TODO brisanje elemenata
// TODO status polje u vgp_parkiranje
// TODO reorganizacija
// TODO balanced BST reda 2 - done
// TODO pametnija reorganizacija baze
// TODO proveriti tip povezivanja prekoračilaca (indirektno)
//
FILE *f        ;
FILE *seq;
FILE *acct;
FILE *idx;

int create_from_data()
{
    // Open CSV data file
    // FILE *f    = fopen("tests/data.csv", "r");
    // FILE *ser  = fopen("serial.db", "wb+");
    FILE *seq  = fopen("sequential.db", "wb+");
    FILE *acct = fopen("main.db", "wb+");
    FILE *idx  = fopen("idx.db", "wb+");

    vgp_parkiranje vgp_e;
    vgp_parkiranje *vgp_arr;

    int LEN = 0;
    int i = 0;


    LEN = csv2ser("tests/data.csv", "serial.db");
    printf("Read from CSV: %d\n", LEN);

    LEN = ser2seq("serial.db", "sequential.db");
    printf("Read from SER: %d\n", LEN);

    LEN = load_ser("serial.db", &vgp_arr);

    seq2act("sequential.db", "act_test");

    return 0;


    /*
    // create main zone
    main_block current_block;
    memset(&current_block.entries, 0, sizeof(vgp_parkiranje)*5);
    current_block.n_overflows = 0;
    current_block.first_overflow = NULL;

    main_block main_blocks_arr[(int)(ceil( (double)LEN/3.))];

    for(i = 0; i < LEN; i++)
    {
        // get 3 entries
        memcpy(&current_block.entries[i%3], &vgp_arr[i], sizeof(vgp_parkiranje) );

        // store block
        if( i%3 == 2 || (i+1) == LEN)
        {
            fwrite(&current_block, sizeof(main_block), 1, acct);
            memcpy( &main_blocks_arr[i/3], &current_block, sizeof(main_block) );

            memset(&current_block.entries, 0, sizeof(vgp_parkiranje)*5);
        }
    }



    // create index zone
    index_block head;
    head.less = NULL;
    head.more = NULL;

    create_bst(&head, main_blocks_arr, 0, ceil(LEN/3.)-1, 0);

    int off = 0;
    puts("\nStore BST\n");
    store_bst( &head, idx, 0, &off);

    puts("\nIndex blocks\n");
    rewind(idx);

    stored_index_block t;
    while( fread(&t, sizeof(stored_index_block), 1, idx ) == 1)
    {
        printf("Key 1: %s %d\n", t.entries[0].key, t.entries[0].block_addr);
        printf("Key 2: %s %d\n", t.entries[1].key, t.entries[1].block_addr);
        printf("Less: %d \t More: %d\n", t.less, t.more);
    }

    puts("\nFind key: 2\n");
    rewind(idx);

    printf("The block addr is: %d\n", search_bst("50273526", idx));
    */


    fclose(seq);
    fclose(acct);
    fclose(idx);

    return 0;
}
