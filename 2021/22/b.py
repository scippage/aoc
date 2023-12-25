from ast import literal_eval
from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from dataclasses import dataclass
from functools import reduce
from itertools import chain, permutations, combinations, product, count, takewhile, islice
from operator import add, mul
from typing import List, Dict, Tuple, Optional
import heapq
import math
import numpy as np
import random
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

instructions = []
pattern = '(on|off) x=(\-?\d+)\.\.(\-?\d+),y=(\-?\d+)\.\.(\-?\d+),z=(\-?\d+)\.\.(\-?\d+)'
for line in lines:
    match = re.match(pattern, line)
    pos = match.group(1)
    x1 = int(match.group(2))
    x2 = int(match.group(3))
    y1 = int(match.group(4))
    y2 = int(match.group(5))
    z1 = int(match.group(6))
    z2 = int(match.group(7))
    
    instructions.append((pos, x1, x2, y1, y2, z1, z2))

print(len(instructions))


def volume(xs, ys, zs):
    x1, x2 = xs
    y1, y2 = ys
    z1, z2 = zs
    return ((x2-x1)+1)*((y2-y1)+1)*((z2-z1)+1)


def intersection(xs, ys, zs):
    coords = (xs, ys, zs)
    res = []
    for coord in coords:
        ca, cb = coord
        a1, a2 = ca
        b1, b2 = cb

        left = max(a1, b1)
        right = min(a2, b2)
        if right < left:
            return None
        else:
            res.append((left, right))

    if len(res) != 3:
        return None
    else:
        return tuple(res)


# k: cube
# v: status
cubes = defaultdict(int)
for ins in tqdm.tqdm(instructions):
    pos, x1, x2, y1, y2, z1, z2 = ins
    xs = (x1, x2)
    ys = (y1, y2)
    zs = (z1, z2)
    
    if pos == 'on':
        switch = 1
    elif pos == 'off':
        switch = -1
    else:
        raise ValueError(pos)

    for k, v in cubes.copy().items():
        kx, ky, kz = k
        overlap = intersection((xs, kx), (ys, ky), (zs, kz))
        if overlap:
            sign = -1 if v > 0 else 1
            cubes[overlap] += sign
    
    for k, v in cubes.copy().items():
        if v == 0:
            del cubes[k]

    # inclusion-exclusion.
    # if pos is off, pretend to conflict but don't add yourself
    if switch == 1:
        cubes[(xs, ys, zs)] += 1

print(cubes)
print(len(cubes))

on = 0
for k, v in cubes.items():
    xs, ys, zs = k
    vol = volume(xs, ys, zs)
    on += vol*v

print(on)
