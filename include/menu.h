#ifndef MENU_H
#define MENU_H

#include <stdio.h>

#define NUM_MENU_ITEMS 5

typedef struct {
    char title[30];
    int (*function)();
} menu_item;

menu_item menu_items_arr[4];

void print_menu();
void init_menu();

#endif
