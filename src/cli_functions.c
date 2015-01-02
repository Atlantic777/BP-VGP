#include "cli_functions.h"
#include <stdio.h>
#include <string.h>
#include "db_file.h"
#include "vgp.h"
#include "utils.h"

#define ROOT_DIR "/tmp/"

int cli_exit_program()
{
    return 0;
}

int cli_create_new_file()
{
    puts("Kreiranje nove datoteke");
    puts("=======================");
    printf("Naziv datoteke: "); scanf("%s", dbf->filename);

    strcpy(dbf->path, ROOT_DIR);
    strcpy(dbf->path+strlen(ROOT_DIR), dbf->filename);

    dbf->file = fopen(dbf->path, "w+");
    fclose(dbf->file);

    return 0;
}

int do_next_entry()
{
    int  resp, ok = 0, i;
    char ans;
    char possible[4] = { 'N', 'Y', 'n', 'y' };

    do {
        printf("Sledeci unos? [Y/n]: ");
        resp = scanf("%c", &ans);

        if(resp == 0)
            return 1;

        if(resp != 1)
            break;

        for(i = 0; i < 4; i++)
        {
            if(ans == possible[i])
            {
                ok = i;
                break;
            }
        }
    } while( !ok );

    return ans%2;
}

int cli_choose_file()
{
    puts("Otvaranje datoteke");
    puts("==================");
    printf("Naziv datoteke: "); scanf("%s", dbf->filename);

    strcpy(dbf->path, ROOT_DIR);
    strcpy(dbf->path+strlen(ROOT_DIR), dbf->filename);

    dbf->file = fopen(dbf->path, "w+");

    return 0;
}

int cli_show_filename()
{
    if(dbf->file == NULL)
        puts("Nije otvorena ni jedna datoteka");
    else
        puts(dbf->filename);

    return 0;
}

int cli_create_serial_file()
{
    if( dbf->file == NULL)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }
    else
    {
        vgp_parkiranje tmp;
        int ok = 1;
        int resp;

        do {
            tmp = create_new_vgp_entry();
        } while( do_next_entry() );
    }
}
