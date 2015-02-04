#include "act_utils.h"
#include "bst_utils.h"
#include <string.h>
#include <stdlib.h>

int build_index_file(db_file *dbf, struct index_entry *keys, int count)
{
    FILE *f_idx = dbf->f_idx;
    int i = 0;

    index_node head;
    head.current = malloc( sizeof(stored_index_block) );
    head.less = NULL;
    head.more = NULL;

    printf("indexing count: %d\n", count);

    // create bst
    create_bst(&head, (index_entry*)keys, 0, count-1, 0);

    puts("starting indexing");
    index_node *current_node;

    int current_offset = 0;

    deq_t *deq = malloc( sizeof(deq_t) );
    enqueue(deq, &head);


    while( deq->head != NULL )
    {
        current_node = dequeue( deq );

        printf("head: %p\n", deq->head);

        printf("[%3d] - %4s %4s", current_offset, current_node->current->entries[0].key, current_node->current->entries[1].key);
        printf(" - less: %12p (%2d) - more %12p (%2d)\n", current_node->less, current_offset*2+1, current_node->more, current_offset*2+2);

        current_node->current->less_offset = -1;
        current_node->current->less_offset = -1;


        if( current_node->less != NULL )
        {
            enqueue( deq, (index_node*)current_node->less );
            current_node->current->less_offset = current_offset*2+1;
        }

        if( current_node->more != NULL)
        {
            enqueue( deq, (index_node*)current_node->more );
            current_node->current->more_offset = current_offset*2+2;
        }

        fwrite( current_node->current, sizeof( stored_index_block ), 1, f_idx );

        current_offset++;
    }


    // destroy bst

    fflush(dbf->f_idx);

    return 0;
}
