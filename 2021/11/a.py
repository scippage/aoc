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

arr = []
for line in lines:
    arr.append([int(c) for c in line])

arr = np.array(arr)
xlim, ylim = arr.shape

def neighbors(pt):
    x, y = pt
    return ((a, b) for (a, b) in 
            ((x+dx, y+dy) for dx in (-1, 0, 1) for dy in (-1, 0, 1) if (dx, dy) != (0, 0))
            if 0 <= a < xlim and 0 <= b < ylim) 

def step(arr):
    fs = 0
    to_flash = []

    for x in range(0, xlim):
        for y in range(0, ylim):
            arr[x][y] += 1
            if arr[x][y] == 10:
                to_flash.append((x, y))
                arr[x][y] = 0
                fs += 1
    
    while len(to_flash) > 0:
        flashing = to_flash
        to_flash = []
        for pt in flashing:
            for neighbor in neighbors(pt):
                x, y = neighbor
                if arr[x][y] != 0:
                    arr[x][y] += 1
                if arr[x][y] == 10:
                    fs += 1
                    arr[x][y] = 0
                    to_flash.append((x, y))

    return fs

flashes = 0
for i in range(100):
    flashes += step(arr)

print(flashes)
