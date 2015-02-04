#ifndef UTILS_H
#define UTILS_H

#include "vgp.h"
#include <time.h>
#include "db_file.h"

vgp_parkiranje create_new_vgp_entry();
void print_vgp_entry(vgp_parkiranje *);

int get_e_br(char*);
int get_reg_br(char*);
int get_epoch(time_t*);
int get_mesto(char*);
int get_boravak(long*);
int str_digit(char *, int);

int do_next_entry();
void vgp_sort(vgp_parkiranje*, int);
int compare_vgp_entries(const void*, const void*);
int load_ser(db_file *dbf, vgp_parkiranje **vgp_arr);
int load_seq(db_file *dbf, vgp_parkiranje **vgp_arr);

#endif
