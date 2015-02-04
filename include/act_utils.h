#ifndef ACT_UTILS_H
#define ACT_UTILS_H

#include "vgp.h"
#include "bst_utils.h"

#define MAIN_BLOCKING_FACTOR 5

struct index_entry;
struct stored_index_block;

typedef struct main_block {
    vgp_parkiranje entries[MAIN_BLOCKING_FACTOR];
    int n_overflows;
    int first_overflow_offset;
} main_block;


int build_main_file(char *file_prefix, vgp_parkiranje *vgp_arr, int count, struct index_entry **keys);
int build_index_file(char *file_prefix, struct index_entry *keys, int count);
void build_overrun_file(char *file_prefix);

int load_idx_block(FILE *f_idx, int n, struct stored_index_block *result);
int load_main_block(FILE *f_main, int n, main_block *result);
int find_block_for_key(FILE *f_idx, char *key);
int find_entry(FILE *f_main, char *key, vgp_parkiranje *result);
int store_key(FILE *f_main, vgp_parkiranje *entry);
int reorganize_db(char *file_prefix);

// print idx_block
// print main_block

#endif
