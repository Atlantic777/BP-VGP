#include "init_db.h"
#include "vgp.h"
#include <stdio.h>
#include "utils.h"
#include "db_functions.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

// TODO brisanje elemenata
// TODO status polje u vgp_parkiranje
// TODO reorganizacija
// TODO balanced BST reda 2 - done
// TODO pametnija reorganizacija baze
// TODO proveriti tip povezivanja prekoračilaca (indirektno)

#define F 5
#define LEN 200

typedef struct {
    vgp_parkiranje entry;
} overflow_block;

typedef struct {
    vgp_parkiranje entries[F];
    int n_overflows;
    overflow_block *first_overflow;
} main_block;

typedef struct {
    int  block_addr;
    char  key[LEN_E_BR+1];
} index_entry;

typedef struct {
    index_entry entries[2];
    int less;
    int more;
} stored_index_block;

typedef struct {
    index_entry entries[2];
    struct index_block *less;
    struct index_block *more;
} index_block;

stored_index_block tmp;

void store_bst(index_block *node, FILE *f, int level, int *offset)
{
    int left  = node->less ? 2*(*offset)+1 : -1;
    int right = node->more ? 2*(*offset)+2 : -1;

    printf("stored on %d, left is %d right is %d\n", *offset, left, right);
    (*offset)++;

    memcpy(tmp.entries, node->entries, sizeof(index_entry)*2);
    tmp.less = left;
    tmp.more = right;

    printf("keys are: %s %s\n", tmp.entries[0].key, tmp.entries[1].key);
    printf("block addrs: %d %d\n", tmp.entries[0].block_addr, tmp.entries[1].block_addr);

    fwrite(&tmp, sizeof(stored_index_block), 1, f);

    if(node->less)
    {
        printf("less %p\n", node->less);
        store_bst((index_block*)node->less, f, level+1, offset);
        left = 2*(*offset)+1;
    }

    if(node->more)
    {
        printf("more %p\n", node->more);
        store_bst((index_block*)node->more, f, level+1, offset);
        right = 2*(*offset)+2;
    }

}

int search_bst(char *key, FILE *f)
{
    int offset = -1;
    stored_index_block tmp, l, m;
    int i;
    int k = atoi(key);
    int e1, e2;

    rewind(f);

    fread(&tmp, sizeof(index_block), 1, f);

    do {
        printf("block addrs: %d %d\n", tmp.entries[0].block_addr, tmp.entries[1].block_addr);
        e1 = (tmp.entries[0].block_addr != -1) ? atoi( tmp.entries[0].key ) : -1;
        e2 = (tmp.entries[1].block_addr != -1) ? atoi( tmp.entries[1].key ) : -1;

        printf("%d %d\n", e1, e2);

        if( (k < e1) && (tmp.less != -1) )
        {
            puts("go less");
            fseek(f, tmp.less*sizeof(stored_index_block), SEEK_SET );
            fread(&tmp, sizeof(stored_index_block), 1, f);
        }
        else if( (k >= e1) && (k < e2) || (e2 == -1) )
        {
            puts("first area");
            return tmp.entries[0].block_addr;
        }
        else if( tmp.more != -1 )
        {
            fseek(f, tmp.more*sizeof(stored_index_block), SEEK_SET );
            fread(&m, sizeof(stored_index_block), 1, f);

            if( k < atoi(m.entries[0].key) )
            {
                puts("second area");
                return tmp.entries[1].block_addr;
            }
            else
            {
                puts("go more");
                tmp = m;
            }

        }
    } while( tmp.less || tmp.more );

    return offset;

}

void create_bst(index_block *head, main_block *main_blocks_arr, int start, int end, int level)
{
    if(start > end) return;

    if( head == NULL ) return;

    int mid = start + (end - start)/2;

    printf("l %d ", level);
    printf("storing info about blocks %d ", mid);


    head->entries[0].block_addr = mid;
    strcpy(head->entries[0].key, main_blocks_arr[mid].entries[0].e_br);

    if(mid+1 <= end)
    {
        printf("%d\n", mid+1);
        head->entries[1].block_addr =  mid+1;
        strcpy(head->entries[1].key, main_blocks_arr[mid+1].entries[0].e_br);
    }
    else
    {
        head->entries[1].block_addr = -1;
        head->entries[1].key[0]     = 0;
        printf("NULL\n");
    }

    if( start < mid )
    {
        head->less = malloc( sizeof(index_block) );
        create_bst( (index_block*)head->less, main_blocks_arr, start, mid-1, level+1 );
    }
    else
    {
        head->less = NULL;
    }

    if( (mid+1) < end )
    {
        head->more = malloc( sizeof(index_block) );
        create_bst( (index_block*)head->more, main_blocks_arr, mid+2, end  , level+1);
    }
    else
    {
        head->less = NULL;
    }
}

int create_from_data()
{
    // Open CSV data file
    FILE *f    = fopen("tests/data.csv", "r");
    FILE *ser  = fopen("serial.db", "wb+");
    FILE *seq  = fopen("sequential.db", "wb+");
    FILE *acct = fopen("main.db", "wb+");
    FILE *idx  = fopen("idx.db", "wb+");

    // For each entry, create struct
    char *tmp = NULL;
    ssize_t len = 100;

    vgp_parkiranje vgp_e;

    // read CSV data
    while( getline(&tmp, &len, f) != -1)
    {
        sscanf(tmp, "%s | %s | %d | %s | %ld",
                    vgp_e.e_br, vgp_e.reg_br, (int*)(&vgp_e.epoch), vgp_e.mesto, &vgp_e.boravak);

        db_store_vgp(ser, &vgp_e);
    }

    // store bin serial data
    rewind(ser);
    vgp_parkiranje vgp_arr[LEN];

    int i = 0;
    while(db_read_vgp(ser, &vgp_e) == 1 )
    {
        memcpy( &vgp_arr[i++], &vgp_e, sizeof(vgp_e) );
    }

    // create sequential data
    vgp_sort(vgp_arr, LEN);
    for(i = 0; i < LEN; i++)
    {
        db_store_vgp(seq, &vgp_arr[i]);
    }

    // read sequential data
    rewind(seq);
    for(i = 0; i < LEN; i++)
    {
        db_read_vgp(seq, &vgp_arr[i]);
    }

    // create main zone
    main_block current_block;
    memset(&current_block.entries, 0, sizeof(vgp_parkiranje)*5);
    current_block.n_overflows = 0;
    current_block.first_overflow = NULL;

    main_block main_blocks_arr[(int)(ceil(LEN/3.))];

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


    fclose(f);
    fclose(ser);
    fclose(seq);
    fclose(acct);
    fclose(idx);

    return 0;
}
