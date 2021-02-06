#!/usr/bin/python2

import sys
sys.path.insert(0, "../../../Utiles/")
from deadpool_dca import *

# On definit de nouveaux filtres pour le formatage des traces. Ils sont inspires de ceux definis par defaut dans le fichier deadpool_dca.
# Ces filtres permettent de prendre en compte le fait que les donnees ont une taille de 8 octets.
mem_addr1_rw8 = Filter('mem_addr1_rw8', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 8, lambda addr, size, data: addr & 0xFF, '<B')
mem_data_rw8 = Filter('mem_data_rw8', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 8, lambda addr, size, data: data, '<Q')



t=TracerGrind('./aes', arch=ARCH.amd64, blocksize=16, addr_range='0x108000-0x4000000', stack_range='0x1ffeffe000-0x1fffffffff',\
 filters = [mem_addr1_rw8, mem_data_rw8])

t.run(100)
bin2daredevil(keywords = [mem_addr1_rw8, mem_data_rw8], configs={'attack_after_sbox':   {'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX'},
                       													  'attack_after_multinv':{'algorithm':'AES', 'position':'LUT/AES_AFTER_MULTINV'}})
