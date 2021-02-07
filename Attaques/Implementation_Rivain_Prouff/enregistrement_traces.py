#!/usr/bin/python2

import sys
sys.path.insert(0, "../../Utiles")
from deadpool_dca import *

try:
	masque = sys.argv[1][0]
except:
	print("Erreur, entree attendu : ./enregistrement_traces nombre_masque")
	quit()
	
def processinput(iblock, blocksize):
    return (None, [masque, '%0*x' % (2*blocksize, iblock)])


t=TracerGrind('./htable/src/mon_main.out', processinput=processinput, arch=ARCH.amd64, blocksize=16, addr_range='0x108000-0x4000000',\
 stack_range='0x1ffefff000-0x1fffffffff')

t.run(50)
bin2daredevil(configs={'attack_after_sbox':   {'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX'},
                       'attack_after_multinv':{'algorithm':'AES', 'position':'LUT/AES_AFTER_MULTINV'}})
