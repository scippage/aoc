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

rows = []
pattern = '.*?#([A-D])#([A-D])#([A-D])#([A-D])#.*?'
for line in lines[2:4]:
    match = re.match(pattern, line)
    r1 = match.group(1)
    r2 = match.group(2)
    r3 = match.group(3)
    r4 = match.group(4)
    rows.append((r1, r2, r3, r4))

print(rows)


