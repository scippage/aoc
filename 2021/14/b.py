from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from functools import reduce
from itertools import chain
import math
import numpy as np
import sys
import tqdm

if len(sys.argv) == 1:
    filename = 'input.txt'
elif sys.argv[1] == 'a':
    filename = 'test_a.txt'
elif sys.argv[1] == 'b':
    filename = 'test_b.txt'
else:
    raise ValueError()
print(filename)

with open(filename) as f:
    lines = f.readlines()
    lines = [line.strip() for line in lines]

template = lines[0]
rules = {}

for line in lines[2:]:
    pair, sub = line.split(' -> ')
    c1, c2 = pair
    rules[(c1, c2)] = sub

def polymerize(polymer_dict):
    tmp_dict = copy(polymer_dict)
    
    for pair, ct in polymer_dict.items():
        if pair in rules:
            c1, c2 = pair
            sub = rules[pair]
            tmp_dict[(c1, sub)] += ct
            tmp_dict[(sub, c2)] += ct
            tmp_dict[pair] -= ct

    return tmp_dict

polymer_dict = Counter([(c1, c2) for c1, c2 in zip(template[:-1], template[1:])])
print(template)
print(polymer_dict)
for i in tqdm.tqdm(range(40)):
    polymer_dict = polymerize(polymer_dict)

print(polymer_dict)

ctr = Counter()
for k, v in polymer_dict.items():
    c1, c2 = k
    if v > 0:
        ctr[c1] += v
        ctr[c2] += v

# everything was double-counted except for first and last edge
ctr[template[0]] += 1
ctr[template[-1]] += 1
for k, v in ctr.items():
    ctr[k] = ctr[k]//2

print(ctr)
print(max(ctr.values()) - min(ctr.values()))
