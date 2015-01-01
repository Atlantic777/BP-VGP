#ifndef VGP_H
#define VGP_H

typedef struct {
    char e_br[10];
    char reg_br[11];
    long epoch;
    char mesto[8];
    long boravak;
} vgp_parkiranje;

void print_menu();


#endif
