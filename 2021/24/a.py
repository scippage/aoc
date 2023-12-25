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
pattern = '(inp|mul|add|mod|div|eql) ([w-z]) ?(-?\d+|[w-z])?'

for line in lines:
    match = re.match(pattern, line)
    op = match.group(1)
    tok1 = match.group(2)
    tok2 = match.group(3)
    instructions.append((op, tok1, tok2))

var = {
    'w': 0,
    'x': 0,
    'y': 0,
    'z': 0,
}

#serial = deque(list('12345578912345'))
#serial = deque(list('53999995829399'))
serial = deque(list('11721151118175'))

for idx, ins in enumerate(instructions):
    op, tok1, tok2 = ins
    if tok2:
        if tok2.isdigit() or (tok2[0] == '-'):
            t2 = int(tok2)
        else:
            t2 = var[tok2]

    if op == 'inp':
        var[tok1] = int(serial.popleft())
        print(var)
    elif op == 'add':
        var[tok1] += t2
    elif op == 'mul':
        var[tok1] *= t2 
    elif op == 'div':
        var[tok1] //= t2
    elif op == 'mod':
        var[tok1] %= t2
    elif op == 'eql':
        var[tok1] = 1 if var[tok1] == t2 else 0
    else:
        raise ValueError(ins)

    #print(ins)
    if (idx % 18) in (4, 5, 15):
        print(ins)

print(var)
