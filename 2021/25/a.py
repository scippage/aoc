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

arr = []
for line in lines:
    arr.append(list(line))
arr = np.array(arr)

ilim, jlim = arr.shape
print(arr)

def step(arr):
    east = set()
    for i in range(ilim):
        for j in range(jlim):
            if arr[i][j] == '>' and arr[i][(j+1) % jlim] == '.':
                east.add((i, j))
    for i, j in east:
        arr[i][j] = '.'
        arr[i][(j+1) % jlim] = '>'

    south = set()
    for i in range(ilim):
        for j in range(jlim):
            if arr[i][j] == 'v' and arr[(i+1) % ilim][j] == '.':
                south.add((i, j))
    for i, j in south:
        arr[i][j] = '.'
        arr[(i+1) % ilim][j] = 'v'

    if len(east) + len(south) == 0:
        return False
    else:
        return True

steps = 0
while True:
    steps += 1
    if not step(arr):
        break

    if steps > 10000:
        break

print(steps)