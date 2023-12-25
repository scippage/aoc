from collections import defaultdict, Counter
from itertools import chain
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

qty_map = {
    0: 6,
    1: 2,
    2: 5,
    3: 5,
    4: 4,
    5: 5,
    6: 6,
    7: 3,
    8: 7,
    9: 6,
}

# uniq: 2, 3, 4, 7 segments corresponding to 1, 7, 4, 8

tot = 0
for parse in parsed:
    sigs, digs = parse
    tot += sum(1 for dig in digs if len(dig) in [2, 3, 4, 7])

print(tot)
