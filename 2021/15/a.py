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

arr = []
for line in lines:
    arr.append([int(c) for c in line])

arr = np.array(arr)
xlim, ylim = arr.shape

def neighbors(pt):
    x, y = pt
    return [(a, b) for (a, b) in
            ((x+1, y), (x-1, y), (x, y+1), (x, y-1)) if
            0 <= a < xlim and 0 <= b < ylim]

visited = set()
q = []
q.append((0, (0, 0), [(0, 0)]))

# dijkstra's without decrease-key
while q:
    priority, pt, path = heapq.heappop(q)
    if pt in visited:
        continue

    if pt == (xlim-1, ylim-1):
        print(path)
        print(priority)
        break

    for neighbor in neighbors(pt):
        x, y = neighbor
        n_wt = arr[x][y]
        heapq.heappush(q, (priority + n_wt, neighbor, path + [neighbor]))

    visited.add(pt)
else:
    raise ValueError("no end")


