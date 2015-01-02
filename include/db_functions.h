#ifndef DB_FUNCTIONS_H
#define DB_FUNCTIONS_H

#include "vgp.h"
#include <stdio.h>

int db_store_vgp(FILE *, vgp_parkiranje *);
int db_read_vgp(FILE *, vgp_parkiranje *);

#endif
