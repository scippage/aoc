from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from functools import reduce
from itertools import chain
from operator import mul
import heapq
import math
import numpy as np
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

# regex to find all possibly negative segments of numbers
# re.findall(r"-?\d+")

line = lines[0][lines[0].index(':'):].strip()
xstr, ystr = line.split(',')
_, xstr = xstr.split('=')
_, ystr = ystr.split('=')
x1, x2 = map(int, xstr.split('..'))
y1, y2 = map(int, ystr.split('..'))


def traj(vx, vy):
    x, y = (0, 0)
    maxy = 0
    path = []
    while True:
        x += vx
        y += vy
        if vx > 0:
            vx -= 1
        elif vx < 0:
            vx += 1
        vy -= 1
        maxy = max(y, maxy)
        path.append((x, y))
        if x1 <= x <= x2 and y1 <= y <= y2:
            #print(path)
            return True, maxy

        # assume targets are x > 0, y < 0.
        if vx <= 0 and y < y1:
            return False, None


maxys = []
vels = []
for a in tqdm.tqdm(range(x2+1)):
    for b in range(y1, -y1+1):
        res, maxy = traj(a, b)
        if res:
            vels.append((a, b))
            maxys.append(maxy)

print(vels)
print(len(vels))
#maxy = max(b for a, b in vels)
print(max(maxys))
