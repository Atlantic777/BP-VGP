#include <stdio.h>
#include "vgp.h"
#include "menu.h"
#include "db_file.h"

int main()
{
    init_menu();

    int resp = 0;
    int end = 0;

    do {
        print_menu();

        scanf("%d", &resp);

        if( resp < 0 || resp >= NUM_MENU_ITEMS )
        {
            puts("Biraj opet!");
        }
        else if ( resp == 0 )
        {
            menu_items_arr[resp].function();
            end = 1;
        }
        else {
            menu_items_arr[resp].function();
        }

    } while( !end );


    return 0;
}
