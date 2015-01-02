#include "db_functions.h"
#include <stdio.h>
#include <string.h>
#include "db_file.h"
#include "vgp.h"
#include "utils.h"

#define ROOT_DIR "/tmp/"

int exit_program()
{
    return 0;
}

int create_new_file()
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

int choose_file()
{
    puts("Otvaranje datoteke");
    puts("==================");
    printf("Naziv datoteke: "); scanf("%s", dbf->filename);

    strcpy(dbf->path, ROOT_DIR);
    strcpy(dbf->path+strlen(ROOT_DIR), dbf->filename);

    dbf->file = fopen(dbf->path, "w+");

    return 0;
}

int show_filename()
{
    if(dbf->file == NULL)
        puts("Nije otvorena ni jedna datoteka");
    else
        puts(dbf->filename);

    return 0;
}

int create_serial_file()
{
    if( dbf->file == NULL)
    {
        puts("Nije otvorena datoteka");
        return -1;
    }
    else
    {
        vgp_parkiranje tmp;
        tmp = create_new_vgp_entry();
    }
}
