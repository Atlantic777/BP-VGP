#include "db_functions.h"
#include <stdio.h>
#include <string.h>

#define ROOT_DIR "/tmp/"

int exit_program()
{
    return 0;
}

int create_new_file()
{
    char filename[30];
    puts("Kreiranje nove datoteke");
    puts("=======================");
    printf("Naziv datoteke: "); scanf("%s", filename);

    char path[50];
    strcpy(path, ROOT_DIR);
    strcpy(path+strlen(ROOT_DIR), filename);

    FILE *f = fopen(path, "w+");
    fclose(f);

    return 0;
}

int choose_file()
{
    char filename[30];
    puts("Otvaranje datoteke");
    puts("==================");
    printf("Naziv datoteke: "); scanf("%s", filename);

    char path[50];
    strcpy(path, ROOT_DIR);
    strcpy(path+strlen(ROOT_DIR), filename);

    FILE *f = fopen(path, "w+");
    fclose(f);

    return 0;
}

int show_filename()
{
    return 0;
}

int create_serial_file()
{
}
