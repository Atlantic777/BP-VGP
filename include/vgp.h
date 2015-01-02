#ifndef VGP_H
#define VGP_H

#include <time.h>

#define LEN_E_BR 9
#define LEN_REG_BR 10
#define LEN_MESTO 7

typedef struct {
    char e_br[LEN_E_BR+1];
    char reg_br[LEN_REG_BR+1];
    time_t epoch;
    char mesto[LEN_MESTO+1];
    long boravak;
} vgp_parkiranje;

#endif
