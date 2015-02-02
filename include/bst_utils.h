#ifndef H_BST_UTILS_
#define H_BST_UTILS_

#include "vgp.h"
#include <stdio.h>
#include "act_utils.h"

struct main_block;

typedef struct {
    vgp_parkiranje entry;
} overflow_block;

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

void create_bst(index_block *head, struct main_block *main_blocks_arr, int start, int end, int level);
int  search_bst(char *key, FILE *f);
void store_bst(index_block *node, FILE *f, int level, int *offset);

// The hell is this?
stored_index_block tmp;


#endif
