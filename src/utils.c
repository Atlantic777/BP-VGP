#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int str_digit(char *s, int len)
{
    int i;

    for(i = 0; i < len; i++)
    {
        if( !isdigit(s[i]) )
            return 0;
    }

    return 1;
}

vgp_parkiranje create_new_vgp_entry()
{
    vgp_parkiranje tmp;

    get_e_br(tmp.e_br);
    get_reg_br(tmp.reg_br);
    get_epoch(&tmp.epoch);
    get_mesto(tmp.mesto);
    get_boravak(&tmp.boravak);

    return tmp;
}

int get_e_br(char *e_br)
{
    char tmp[LEN_E_BR];
    int ok = 0;

    do {
        printf("Evidencioni broj: ");
        scanf("%s", tmp);

        int len = strlen(tmp);

        if(len != LEN_E_BR || !str_digit(tmp, len) )
        {
            puts("Evidencioni broj se sastoji od tacno 9 cifara.");
            puts("Upisi ponovo.");
            break;
        }
        else
        {
            strcpy(e_br, tmp);
            ok = 1;
        }
    } while( !ok );

    return 1;
}

int get_reg_br(char *reg_br)
{
    char tmp[LEN_REG_BR];

    printf("Registarska oznaka: ");
    scanf("%s", tmp);

    int len = strlen(tmp);

    if(len > LEN_REG_BR)
    {
        printf("Registarska oznaka ne moze biti duza od %d karaktera.\n", LEN_REG_BR);
        puts("Upisi ponovo.");
        return 0;
    }
    else
    {
        strcpy(reg_br, tmp);
        return 1;
    }
}

int get_epoch(time_t *epoch)
{
    //TODO fix time entry
    printf("Datum i vreme parkiranja: \n");
    time(epoch);
    puts(ctime(epoch));
    return 1;
}

int get_mesto(char *mesto)
{
    char tmp[LEN_MESTO];

    printf("Oznaka parking mesta: ");
    scanf("%s", tmp);

    int len = strlen(tmp);

    if(len > LEN_MESTO)
    {
        printf("Oznaka parking mesta ne moze biti duza od %d karaktera\n", LEN_MESTO);
        return 0;
    }
    else
    {
        strcpy(mesto, tmp);
        return 1;
    }
}

int get_boravak(long *boravak)
{
    long tmp;
    printf("Dužina boravka [min]: ");
    scanf("%ld", &tmp);

    if(tmp <= 0 || tmp > 1000000)
    {
        puts("Vreme boravka moze biti od 0 do 1 000 000 minuta.");
        return 0;
    }
    else
    {
        *boravak = tmp;
        return 1;
    }
}