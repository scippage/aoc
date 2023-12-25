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
    
    if not (all(-50 <= p for p in (x1, y1, z1)) and all(p <= 50 for p in (x2, y2, z2))):
        continue
    instructions.append((pos, x1, x2, y1, y2, z1, z2))

print(len(instructions))


def cuboid(x1, x2, y1, y2, z1, z2):
    for x in range(x1, x2+1):
        for y in range(y1, y2+1):
            for z in range(z1, z2+1):
                yield (x, y, z)

on = set()
for ins in tqdm.tqdm(instructions):
    pos, x1, x2, y1, y2, z1, z2 = ins
    if pos == 'on':
        for pt in cuboid(x1, x2, y1, y2, z1, z2):
            on.add(pt)
    else:
        for pt in cuboid(x1, x2, y1, y2, z1, z2):
            if pt in on:
                on.remove(pt)

print(len(on))
