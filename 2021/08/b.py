from collections import defaultdict, Counter
from copy import copy
from itertools import chain, permutations
import math
import numpy as np

with open('input.txt') as f:
    lines = f.readlines()

parsed = []

for line in lines:
    line = line.strip()
    sigs, digs = line.split('|')
    sigs = sigs.split()
    digs = digs.split()
    parsed.append((sigs, digs))

glow_map = {
    0: 'abcefg',
    1: 'cf',
    2: 'acdeg',
    3: 'acdfg',
    4: 'bcdf',
    5: 'abdfg',
    6: 'abdefg',
    7: 'acf',
    8: 'abcdefg',
    9: 'abcdfg',
}

rev_glow_map = {v: k for (k, v) in glow_map.items()}

# uniq: 2, 3, 4, 7 segments corresponding to 1, 7, 4, 8
# linalg approach failed; 7! = 720*7 = 5040 so just try them all

def apply_map(perm, seq):
    fwd_map = {k: v for (k, v) in zip('abcdefg', perm)}
    img = ''.join((fwd_map[c] for c in seq))
    return img
        
tot = 0
for parse in parsed:
    sigs, digs = parse
    sig_values = set(''.join(sorted(sig)) for sig in sigs)

    for perm in permutations('abcdefg'):
        for num, seq in glow_map.items():
            mapped_seq = apply_map(perm, seq)
            if ''.join(sorted(mapped_seq)) not in sig_values:
                break
        else:
            # this is the correct perm
            decoded = 0
            for dig in digs:
                rev_map = {k: v for (k, v) in zip(perm, 'abcdefg')}
                preimg = ''.join(rev_map[c] for c in dig)
                decoded_dig = rev_glow_map[''.join(sorted(preimg))]
                decoded = decoded*10 + decoded_dig
            tot += decoded

print(tot)
