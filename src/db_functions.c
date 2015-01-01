#include "db_functions.h"
#include <stdio.h>
#include <string.h>
#include "db_file.h"

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

    char path[50];
    strcpy(path, ROOT_DIR);
    strcpy(path+strlen(ROOT_DIR), dbf->filename);

    dbf->file = fopen(path, "w+");
    fclose(dbf->file);

    return 0;
}

int choose_file()
{
    puts("Otvaranje datoteke");
    puts("==================");
    printf("Naziv datoteke: "); scanf("%s", dbf->filename);

    char path[50];
    strcpy(path, ROOT_DIR);
    strcpy(path+strlen(ROOT_DIR), dbf->filename);

    dbf->file = fopen(path, "w+");
    fclose(dbf->file);

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
    // Proveri da li je otvorena datoteka
    // Petlja za dodavanje sloga

    // TODO dodati funkciju za upis jednog sloga
}
