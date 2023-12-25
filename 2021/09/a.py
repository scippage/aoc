from collections import defaultdict, Counter
from itertools import chain
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

# asymptotically the same
lows = [pt for pt in [(x,y) for x in range(0,xlim) for y in range(0, ylim)] if all(arr[pt] < arr[neighbor] for neighbor in neighbors(pt))]
risk = sum(arr[low]+1 for low in lows)
print(lows)
print(risk)


