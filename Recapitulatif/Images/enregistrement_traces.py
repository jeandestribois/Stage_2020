#!/usr/bin/python2

from deadpool_dca import *


def processinput(iblock, blocksize):
    return (None, ['%0*x' % (2*blocksize, iblock)])

def processoutput(output, blocksize):
    return int(output, 16)

t = TracerGrind('./mon_main.out', processinput=processinput, processoutput=processoutput, arch=ARCH.amd64, blocksize=16, addr_range='0x108000-0x4000000', stack_range='0x1ffefff000-0x1fffffffff')

t.run(1000)

bin2daredevil(configs={'attack_after_sbox':   {'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX'}, 'attack_after_multinv':{'algorithm':'AES', 'position':'LUT/AES_AFTER_MULTINV'},})