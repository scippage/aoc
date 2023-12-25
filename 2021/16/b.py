from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from functools import reduce
from itertools import chain
from operator import mul
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

# want each hex digit to take up 4 spaces, so 6 = 0b0110 and not 0b110. 2 extra spaces for the 0b.
raw = format(int(lines[0], 16), f'#0{2+line_len*4}b')
versions = []

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
    else:
        len_type_id = int(bits[0])
        pkt_len += 1

        bits = bits[1:]
        stack = []
        if len_type_id == 0:
            subpkt_len = int(bits[:15], 2)
            pkt_len += 15 + subpkt_len

            bits = bits[15:]
            processed_bits = 0
            while processed_bits < subpkt_len:
                s_ver, s_type, s_val, s_len = parse(bits, type_id)
                processed_bits += s_len
                bits = bits[s_len:]
                stack.append(s_val)
        elif len_type_id == 1:
            num_subpkts = int(bits[:11], 2)
            pkt_len += 11

            bits = bits[11:]
            for i in range(num_subpkts):
                s_ver, s_type, s_val, s_len = parse(bits, type_id)
                pkt_len += s_len
                bits = bits[s_len:]
                stack.append(s_val)

        # unwind stack
        if type_id == 0:
            number = sum(stack)
        elif type_id == 1:
            number = reduce(mul, stack)
        elif type_id == 2:
            number = min(stack)
        elif type_id == 3:
            number = max(stack)
        elif type_id == 5:
            number = 1 if stack[0] > stack[1] else 0
        elif type_id == 6:
            number = 1 if stack[0] < stack[1] else 0
        elif type_id == 7:
            number = 1 if stack[0] == stack[1] else 0

    return version, type_id, number, pkt_len

f_ver, f_type, f_val, f_len = parse(raw[2:], 0)

print(f"final num: {f_val}")
print(f_len)
print(f'versions: {versions}')
print(f'version_sum: {sum(versions)}')
