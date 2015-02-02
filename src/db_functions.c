#include "db_functions.h"
#include <stdio.h>
#include "vgp.h"

int db_store_vgp(FILE *f, vgp_parkiranje *entry)
{
    return fwrite(entry, sizeof(vgp_parkiranje), 1, f);
}

int db_read_vgp(FILE *f, vgp_parkiranje *entry)
{
    return fread(entry, sizeof(vgp_parkiranje), 1, f);
}
