from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from functools import reduce
from itertools import chain
import math
import numpy as np
import sys

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
    rules[pair] = sub

def polymerize(polymer):
    result = []
    for c1, c2 in zip(polymer[:-1], polymer[1:]):
        result.append(c1)
        if c1+c2 in rules:
            result.append(rules[c1+c2])
    result.append(polymer[-1])
    return ''.join(result)

polymer = template
for i in range(10):
    polymer = polymerize(polymer)

ctr = Counter(polymer)
print(polymer)
print(ctr)
print(max(ctr.values()) - min(ctr.values()))
