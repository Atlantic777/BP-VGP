#!/usr/bin/python
from random import randint, choice
from time import time
from uuid import uuid4
from os import sys

len_e   = 9
len_reg = 11

max_e   = int('9'*len_e)
max_reg = int('9'*len_reg)

time_offset = 600

gradovi = ['NoviSad', 'Kula', 'Beograd', 'BrBanja']

def gen_evidencija():
    ev = randint(0, max_e);
    return str(ev);

def gen_reg_br():
    return str(uuid4())[:len_reg]

def gen_epoch():
    return str( int(time()) + randint(-time_offset,time_offset))

def gen_one_vgp():
    ev      = gen_evidencija()
    reg_br  = gen_reg_br()
    epoch   = gen_epoch()
    mesto   = choice(gradovi)
    boravak = str(randint(0,time_offset))

    return "\t | \t".join( [ev, reg_br, epoch, mesto, boravak] )

if __name__ == "__main__":
    entries = 10
    if len(sys.argv) == 2:
        entries = int(sys.argv[1])

    for i in range(entries):
        print gen_one_vgp()
