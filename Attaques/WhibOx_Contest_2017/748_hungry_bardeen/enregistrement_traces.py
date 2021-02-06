#!/usr/bin/python2

import sys
sys.path.insert(0, "../../../Utiles/")
from deadpool_dca import *


t=TracerGrind('./aes', arch=ARCH.amd64, blocksize=16, addr_range='0x0-0x1fffffffff', stack_range='0x1ffeffe000-0x1fffffffff')
t.run(200)
bin2daredevil(configs={'attack_after_sbox':   {'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX'},
                       'attack_after_multinv':{'algorithm':'AES', 'position':'LUT/AES_AFTER_MULTINV'}})
