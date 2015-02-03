#include "vgp.h"
#include "bst_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void store_bst(index_node *node, FILE *f, int level, int *offset)
{
    /*
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
    */

}

int search_bst(char *key, FILE *f)
{
    /*
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
    */

    return -1;
}


void create_bst(index_node *head, index_entry *keys, int start, int end, int level)
{
    // what does this check do?
    if(start > end) return;

    // what does this check do?
    if( head == NULL ) return;

    // get into the middle
    int mid = start + (end - start)/2;


    // print shitty info
    printf("l %d ", level);
    printf("storing info about blocks %d ", mid);


    // set offset into the current node. Current has to exist!
    head->current = malloc( sizeof( index_entry ) * 2);
    head->current->entries[0].block_addr = mid;
    strcpy(head->current->entries[0].key, keys[mid].key);


    if(mid+1 <= end)
    {
        printf("%d\n", mid+1);
        head->current->entries[1].block_addr =  mid+1;
        strcpy(head->current->entries[1].key, keys[mid+1].key);
    }
    else
    {
        head->current->entries[1].block_addr = -1;
        head->current->entries[1].key[0]     = 0;
        printf("NULL\n");
    }

    if( start < mid )
    {
        head->less = malloc( sizeof(index_node) );
        create_bst( (index_node*)head->less, keys, start, mid-1, level+1 );
    }
    else
    {
        head->less = NULL;
    }


    if( (mid+1) < end )
    {
        head->more = malloc( sizeof(index_node) );
        create_bst( (index_node*)head->more, keys, mid+2, end  , level+1);
    }
    else
    {
        head->more = NULL;
    }
}

void enqueue(deq_t *deq, index_node *new_node)
{
    deq_node *new_tail_node = malloc( sizeof(deq_node) );

    new_tail_node->data = new_node;
    new_tail_node->next = NULL;

    if( deq->tail != NULL)
    {
        deq->tail->next = new_tail_node;
    }
    else
    {
        deq->head = new_tail_node;
    }
    deq->tail = new_tail_node;

}

index_node* dequeue(deq_t *deq)
{
    deq_node *old_head = deq->head;
    index_node *ret_data = deq->head->data;

    if( deq->head == deq->tail)
        deq->tail = NULL;

    deq->head = deq->head->next;

    free(old_head);

    return ret_data;
}
