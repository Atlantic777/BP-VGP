#ifndef MENU_H
#define MENU_H

#include <stdio.h>

#define NUM_MENU_ITEMS 7

typedef struct {
    char title[30];
    int (*function)();
} menu_item;

menu_item menu_items_arr[NUM_MENU_ITEMS];

void print_menu();
void init_menu();

#endif
