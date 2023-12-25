from ast import literal_eval
from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from dataclasses import dataclass
from functools import reduce
from itertools import chain, permutations, combinations, product
from operator import add, mul
from typing import List, Dict, Tuple, Optional
import heapq
import math
import numpy as np
import re
import sys
import tqdm

if len(sys.argv) == 1:
    filename = 'input.txt'
else:
    arg = sys.argv[1]
    filename = f'test_{arg}.txt'
print(filename)

with open(filename) as f:
    lines = f.readlines()
    lines = [line.strip() for line in lines]

algo = lines[0]
algo = [1 if c == '#' else 0 for c in algo]
modulate = algo[0] == 1 and algo[-1] == 0

arr = []
for line in lines[2:]:
    row = [1 if c == '#' else 0 for c in line]
    arr.append(row)

arr = np.array(arr, dtype=int)
print(arr)
xlim, ylim = arr.shape

img = {}
one_locs = set()
for x in range(xlim):
    for y in range(ylim):
        img[(y, -x)] = arr[x][y]

        one_locs.add((y, -x))
        #if arr[x][y] == 1:
        #    one_locs.add((y, -x))

def window(pt):
    x, y = pt
    return [(x-1, y+1), (x, y+1), (x+1, y+1), (x-1, y), (x, y), (x+1, y), (x-1, y-1), (x, y-1), (x+1, y-1)]

def to_process(locs):
    max_x = max(x for x, y in locs)
    min_x = min(x for x, y in locs)
    max_y = max(y for x, y in locs)
    min_y = min(y for x, y in locs)
    print(max_x, min_x, max_y, min_y)
    return ((a, b) for a in range(min_x-1, max_x+2) for b in range(min_y-1, max_y+2))

def to_process2(locs):
    border = set()
    for pt in locs:
        for border_pt in window(pt):
            border.add(border_pt)
    return border

def enhance(img, locs, i):
    new_img = {}
    new_locs = set()
    for pt in locs:
        if modulate:
            # if a pt is not in img, then it must have been a void 0 the previous iteration.
            # else it would have been in the previous iteration's locs and thus become part of img
            index = [img[(a, b)] if (a, b) in img else (i % 2) for (a, b) in window(pt)]
        else:
            index = [img[(a, b)] if (a, b) in img else 0 for (a, b) in window(pt)]
        index = ''.join((str(c) for c in index))
        index = int(index, 2)
        pixel = algo[index]
        new_img[pt] = pixel
        new_locs.add(pt)
        #if pixel == 1:
        #    new_locs.add(pt)
    new_locs = to_process(new_locs)
    return new_img, new_locs

def draw(img):
    max_x = max(x for x, y in img.keys())
    min_x = min(x for x, y in img.keys())
    max_y = max(y for x, y in img.keys())
    min_y = min(y for x, y in img.keys())
    xlim = max_x - min_x + 1
    ylim = max_y - min_y + 1
    drawn = np.zeros((xlim, ylim), dtype=int)
    for pt, pixel in img.items():
        x, y = pt
        a = x - min_x
        b = -(max_y - y)
        drawn[-b][a] = pixel
    return drawn

print(modulate)
locs = to_process(one_locs)
for i in range(50):
    #print(sum(img.values()))
    if not modulate:
        print(draw(img))
    img, locs = enhance(img, locs, i)

if not modulate:
    print(draw(img))

print(sum(img.values()))
