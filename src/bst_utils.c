#include "vgp.h"
#include "bst_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
        strcpy(head->current->entries[1].key, "NONE");
        head->less = NULL;
        head->more = NULL;
        printf("NULL\n");
        return;
    }


    head->less = malloc( sizeof(index_node) );
    ((index_node*)head->less)->current = malloc( sizeof(stored_index_block));

    if( start < mid )
    {
        create_bst( (index_node*)head->less, keys, start, mid-1, level+1 );
    }
    else
    {
        strcpy( ((index_node*)head->less)->current->entries[0].key, "NONE" );
        ((index_node*)head->less)->current->entries[0].block_addr = -1;

        strcpy( ((index_node*)head->less)->current->entries[1].key, "NONE" );
        ((index_node*)head->less)->current->entries[1].block_addr = -1;
    }


    head->more = malloc( sizeof(index_node) );
    ((index_node*)head->more)->current = malloc( sizeof(stored_index_block) );
    if( (mid+1) < end )
    {
        create_bst( (index_node*)head->more, keys, mid+2, end  , level+1);
    }
    else
    {
        strcpy( ((index_node*)head->more)->current->entries[0].key, "NONE" );
        ((index_node*)head->more)->current->entries[0].block_addr = -1;

        strcpy( ((index_node*)head->more)->current->entries[1].key, "NONE" );
        ((index_node*)head->more)->current->entries[1].block_addr = -1;
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
