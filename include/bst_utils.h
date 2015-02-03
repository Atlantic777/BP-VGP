#ifndef H_BST_UTILS_
#define H_BST_UTILS_

#include "vgp.h"
#include <stdio.h>
#include "act_utils.h"

struct main_block;

typedef struct {
    vgp_parkiranje entry;
    int next_entry_offset;
} overflow_block;

typedef struct {
    int  block_addr;
    char  key[LEN_E_BR+1];
} index_entry;

typedef struct {
    index_entry entries[2];
    int less_offset;
    int more_offset;
} stored_index_block;

typedef struct {
    stored_index_block *current;
    struct index_node *less;
    struct index_node *more;
} index_node;

typedef struct deq_node {
    index_node *data;
    struct deq_node *next;
} deq_node;

typedef struct {
    deq_node *head;
    deq_node *tail;
} deq_t;

void enqueue(deq_t *deq, index_node *new_node);
index_node* dequeue(deq_t *deq);

void create_bst(index_node *head, index_entry *keys, int start, int end, int level);

#endif
