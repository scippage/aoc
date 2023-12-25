from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from functools import reduce
from itertools import chain
import math
import numpy as np
import sys

if len(sys.argv) == 1:
    filename = 'input.txt'
else:
    filename = 'test.txt'
print(filename)

with open(filename) as f:
    lines = f.readlines()
    lines = [line.strip() for line in lines]

scores = {
    ')': 3,
    ']': 57,
    '}': 1197,
    '>': 25137,
}

qs = {
    ')': deque(),
    ']': deque(),
    '}': deque(),
    '>': deque(),
}

opens = {
    '(': ')',
    '[': ']',
    '{': '}',
    '<': '>',
}

score = 0
for line in lines:
    qs_cpy = deepcopy(qs)
    for idx, c in enumerate(line):
        if c in opens.keys():
            qs_cpy[opens[c]].append(idx)
        elif c in qs_cpy.keys():
            # bad if no open
            if not qs_cpy[c]:
                score += scores[c]
                break
            popped_idx = qs_cpy[c].pop()
            
            if any(v[-1] > popped_idx for v in qs_cpy.values() if v):
                # bad if unclosed
                score += scores[c]
                break
        else:
            raise ValueError(c)

print(score)
