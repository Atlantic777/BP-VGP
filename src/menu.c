#include "menu.h"
#include "cli_functions.h"
#include <string.h>
#include <stdio.h>

void print_menu()
{
    int i;
    for(i = 1; i < NUM_MENU_ITEMS; i++)
    {
        printf("%d. %s\n", i, menu_items_arr[i].title);
    }
    printf("%d. %s\n", 0, menu_items_arr[0].title);

    printf("> ");

}

void init_menu()
{
    char menu_item_titles[NUM_MENU_ITEMS][35] = {
            "Izlaz iz programa.",
            "Formiraj novu datoteku",
            "Izaberi aktivnu datoteku.",
            "Prikazi naziv aktivne datoteke.",
            "Kreiraj serijsku datoteku.",
            "Kreiraj bazu iz podataka.",
            "Prikazi index",
        };

    void *menu_item_functions[NUM_MENU_ITEMS] = {
            cli_exit_program,
            cli_create_new_file,
            cli_choose_file,
            cli_show_filename,
            cli_create_serial_file,
            cli_create_from_data,
            cli_print_idx,
        };

    int i;
    for(i = 0; i < NUM_MENU_ITEMS; i++)
    {
        strcpy(menu_items_arr[i].title, menu_item_titles[i]);
        menu_items_arr[i].function = menu_item_functions[i];
    }

}
