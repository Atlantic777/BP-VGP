#include "db_functions.h"
#include <stdio.h>
#include "vgp.h"

int db_store_vgp(FILE *f, vgp_parkiranje *entry)
{
    fwrite(entry, sizeof(vgp_parkiranje), 1, f);
    return 0;
}

int db_read_vgp(FILE *f, vgp_parkiranje *entry)
{
    fread(entry, sizeof(vgp_parkiranje), 1, f);
    return 0;
}
