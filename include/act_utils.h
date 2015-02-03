#ifndef ACT_UTILS_H
#define ACT_UTILS_H

#include "vgp.h"
#include "bst_utils.h"

#define MAIN_BLOCKING_FACTOR 5

struct index_entry;

typedef struct main_block {
    vgp_parkiranje entries[MAIN_BLOCKING_FACTOR];
    int n_overflows;
    int first_overflow_offset;
} main_block;


int build_main_file(char *file_prefix, vgp_parkiranje *vgp_arr, int count, struct index_entry **keys);
int build_index_file(char *file_prefix, struct index_entry *keys, int count);

#endif
