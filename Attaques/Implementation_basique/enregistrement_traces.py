#!/usr/bin/python2

import sys
sys.path.insert(0, '../../Utiles/')
from deadpool_dca import *


t=TracerGrind('./AES/aes', arch=ARCH.amd64, blocksize=16, addr_range='0x108000-0x4000000, 0x402d000-0x4830000, 0x484d000-0x4852000', stack_range='0x1ffefff000-0x1fffffffff')

t.run(30)
bin2daredevil(configs={'attack_after_sbox':   {'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX'},
                       'attack_after_multinv':{'algorithm':'AES', 'position':'LUT/AES_AFTER_MULTINV'}})
