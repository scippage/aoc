from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from functools import reduce
from itertools import chain
import heapq
import math
import numpy as np
import sys

if len(sys.argv) == 1:
    filename = 'input.txt'
else:
    arg = sys.argv[1]
    filename = f'test_{arg}.txt'
print(filename)

with open(filename) as f:
    lines = f.readlines()
    lines = [line.strip() for line in lines]


line_len = len(lines[0])
raw = format(int(lines[0], 16), f'#0{2+line_len*4}b')
versions = []
numbers = []
print(lines[0])
print(raw)

# skip 0b and lex/parse
def parse(bits, mode = None):
    version = int(bits[:3], 2)
    type_id = int(bits[3:6], 2)
    number = None
    pkt_len = 6
    versions.append(version)
    
    bits = bits[6:]
    if type_id == 4: # literal
        number = ""
        while bits:
            grp = bits[:5]

            more, data = grp[0], grp[1:]
            number += data
            pkt_len += 5
            if int(more) == 0:
                break
            bits = bits[5:]
        
        number = int(number, 2)
        numbers.append(number)
    else:
        len_type_id = int(bits[0])
        pkt_len += 1

        bits = bits[1:]
        if len_type_id == 0:
            subpkt_len = int(bits[:15], 2)
            pkt_len += 15 + subpkt_len

            bits = bits[15:]
            processed_bits = 0
            while processed_bits < subpkt_len:
                s_ver, s_type, s_val, s_len = parse(bits, ("SUBPKT_LEN", (processed_bits, subpkt_len)))
                processed_bits += s_len
                bits = bits[s_len:]
        elif len_type_id == 1:
            num_subpkts = int(bits[:11], 2)
            pkt_len += 11

            bits = bits[11:]
            for i in range(num_subpkts):
                s_ver, s_type, s_val, s_len = parse(bits, ("NUM_SUBPKTS", (i, num_subpkts)))
                pkt_len += s_len
                bits = bits[s_len:]

    return version, type_id, number, pkt_len

f_ver, f_type, f_val, f_len = parse(raw[2:])

print(f_val)
print(f_len)
print(f'versions: {versions}')
print(f'version_sum: {sum(versions)}')
