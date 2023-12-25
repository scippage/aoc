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

coords = []
folds = []
for idx in range(len(lines)):
    line = lines[idx]
    if not line:
        break
    coords.append([int(c) for c in line.split(',')])

for idx in range(idx+1, len(lines)):
    line = lines[idx]
    fst, snd = line.split('=')
    fst = fst[-1]
    snd = int(snd)
    folds.append((fst, snd))

xlim = max(x for (x, y) in coords)
ylim = max(y for (x, y) in coords)

arr = np.zeros((xlim+1, ylim+1), dtype=np.int8)
for x, y in coords:
    arr[x][y] = 1

def fold(arr, axis, val):
    xl, yl = arr.shape
    if axis == 'x':
        flipped = np.flip(arr[val+1:], 0)
        base = arr[:val]
        
        padding = base.shape[0] - flipped.shape[0]
        if padding > 0:
            # pad bottom side
            flipped = np.pad(flipped, [(0, padding), (0, 0)])
        elif padding < 0:
            base = np.pad(base, [(-padding, 0), (0, 0)])
    elif axis == 'y':
        flipped = np.flip(arr[:, val+1:], 1)
        base = arr[:, :val]
        
        padding = base.shape[1] - flipped.shape[1]
        if padding > 0:
            # pad bottom side
            flipped = np.pad(flipped, [(0, 0), (0, padding)])
        elif padding < 0:
            base = np.pad(base, [(0, 0), (-padding, 0)])
    else:
        raise ValueError(val)
    
    arr = np.bitwise_or(base, flipped)
    return arr

for axis, val in folds[:1]:
    arr = fold(arr, axis, val)

print(arr)
print(arr.shape)
print(np.sum(arr))
