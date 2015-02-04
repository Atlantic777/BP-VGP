#include "seq2act.h"
#include <stdlib.h>
#include "vgp.h"
#include "utils.h"
#include "act_utils.h"
#include "bst_utils.h"

int seq2act(db_file *dbf)
{
    vgp_parkiranje *vgp_arr;
    int count = load_seq(dbf, &vgp_arr);
    index_entry *keys;

    int n_blocks = build_main_file(dbf, vgp_arr, count, (struct index_entry**)&keys);
    build_index_file(dbf, (struct index_entry*)keys, count/3);

    // build_overrun_file(prefix_act);

    //open_db_files( dbf, dbf->path, dbf->f_prefix );

    free(vgp_arr);
    free(keys);

    return n_blocks;
}
