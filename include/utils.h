#ifndef UTILS_H
#define UTILS_H

#include "vgp.h"
#include <time.h>

vgp_parkiranje create_new_vgp_entry();

int get_e_br(char*);
int get_reg_br(char*);
int get_epoch(time_t*);
int get_mesto(char*);
int get_boravak(long*);
int str_digit(char *, int);

int do_next_entry();

#endif