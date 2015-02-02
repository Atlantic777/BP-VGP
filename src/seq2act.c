#include "seq2act.h"
#include <stdlib.h>
#include "vgp.h"
#include "utils.h"
#include "act_utils.h"
#include "bst_utils.h"

int seq2act(char *filename_seq, char *prefix_act)
{
    vgp_parkiranje *vgp_arr;
    int count = load_seq(filename_seq, &vgp_arr);
    index_entry *keys;

    int n_blocks = build_main_file(prefix_act, vgp_arr, count, (struct index_entry**)&keys);
    //build_index_file();
    //build_overrun_file();

    free(vgp_arr);
    free(keys);

    return n_blocks;
}
