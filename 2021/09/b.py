from collections import defaultdict, Counter
from itertools import chain
from functools import reduce
import math
import numpy as np

with open('input.txt') as f:
    lines = f.readlines()

heights = []
for line in lines:
    heights.append(line.strip())

arr = []
for height in heights:
    row = [int(c) for c in height]
    arr.append(row)

arr = np.array(arr)
xlim, ylim = arr.shape

def neighbors(pt):
    x, y = pt
    return ((a, b) for (a, b) in ((x+1, y), (x-1, y), (x, y-1), (x, y+1)) if 0 <= a < xlim and 0 <= b < ylim)

visited = set()
basins = []

# asymptotically the same
lows = [pt for pt in [(x, y) for x in range(0, xlim) for y in range(0, ylim)] if all(arr[pt] < arr[neighbor] for neighbor in neighbors(pt))]

def visit(pt, basin, level):
    if pt in visited:
        return
    #if arr[pt] != level+1 or arr[pt] == 9:
    if arr[pt] < level or arr[pt] == 9:
        return

    visited.add(pt)
    basin.append(pt)
    for neighbor in neighbors(pt):
        visit(neighbor, basin, arr[pt])


for low in lows:
    visited.add(low)
    basin = [low]
    for neighbor in neighbors(low):
        visit(neighbor, basin, arr[low])
    basins.append(basin)


print(len(basins))
sorted_basins = list(reversed(sorted(basins, key=len)))
top3 = [len(basin) for basin in sorted_basins[:3]]
print(top3)
print(reduce(lambda acc, x: acc*x, top3))

def draw(basin):
    drawing = np.zeros((100, 100))
    for x, y in basin:
        drawing[x][y] = arr[x][y]
    return drawing

#np.set_printoptions(threshold=10000)
print(sorted_basins[0])
#print(draw(sorted_basins[0]))
