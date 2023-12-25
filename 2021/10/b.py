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

scores = {
    ')': 1,
    ']': 2,
    '}': 3,
    '>': 4,
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

points = []
for line in lines:
    qs_cpy = deepcopy(qs)
    bad = False
    for idx, c in enumerate(line):
        if c in opens.keys():
            qs_cpy[opens[c]].append(idx)
        elif c in qs_cpy.keys():
            # bad if no open
            if not qs_cpy[c]:
                bad = True
                break
            popped_idx = qs_cpy[c].pop()
            
            if any(v[-1] > popped_idx for v in qs_cpy.values() if v):
                # bad if unclosed
                bad = True
                break
        else:
            raise ValueError(c)

    if not bad:
        heap = []
        for k, v in qs_cpy.items():
            for vidx in v:
                heapq.heappush(heap, (vidx, k))

        completion = []
        while heap:
            vidx, k = heapq.heappop(heap)
            completion.append(k)
        completion.reverse()
        point = 0
        for c in completion:
            point = point*5 + scores[c]
        points.append(point)

print(points)
points = sorted(points)
print(points[len(points)//2])
