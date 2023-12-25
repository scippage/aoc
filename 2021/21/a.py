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

p1_pos = int(re.match('Player 1 starting position: (\d+)', lines[0]).group(1))
p2_pos = int(re.match('Player 2 starting position: (\d+)', lines[1]).group(1))

pos = [p1_pos, p2_pos]
scores = [0, 0]

die = count(1)
plies = 0
while True:
    i = plies % 2
    rolls = islice(die, 0, 3)
    pos[i] = ((pos[i]-1) + sum(rolls)) % 10 + 1
    scores[i] += pos[i]
    if scores[i] >= 1000:
        plies += 1
        break
    plies += 1

print(f"{plies}, {plies*3}")
print(scores)

print(scores[plies%2] * plies * 3)
