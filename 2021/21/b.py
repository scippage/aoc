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
print(f'starting pos: {pos}')

# how many ways (worlds) are there to get your roll (jump)?
# stars and bars with constraints...get general solution later?
# *|**|***
# count multisets of 3 positive ints that add to 6: (n-1)C(k-1) = (6-1)C(3-1) = 10
# now we require one [specific] member doesn't have sum greater than 3. so earmark 3 to one side, leaving 3, and count the same for (n, k) = (6-3, 3). So (n-1)C(k-1) = 3.
# subtract 10-3 and the answer is 7.
#
# recall also the soln to counting multisets of non-negative ints
# |**||****
# the positive bins solution is like earmarking k elements, such that each bin has 1 element, and then computing for (n, k) = (n-k, k)
# so in this solution we add an extra k to the set we draw from instead.
# (n+k+1)C(k-1)
#
# what if our constraint was a different max for each bin?
# https://math.stackexchange.com/questions/1922819/stars-and-bars-with-bounds
# https://math.stackexchange.com/questions/756462/error-solving-stars-and-bars-type-problem/756501#756501
# generating functions are an easier general way to solve

jump_to_worlds = {
    3: 1,
    4: 3,
    5: 6,  # 113*3, 122*3
    6: 7,  # 123*6, 222
    7: 6,
    8: 3,
    9: 1,
}


# num of ways to get to state (pos, scores, i)
# count all winning states

ways = defaultdict(int)
ways[(pos[0], pos[1], scores[0], scores[1], 0)] = 1


def compute(p0, p1, s0, s1, i):
    if (p0, p1, s0, s1, i) in ways:
        return ways[(p0, p1, s0, s1, i)]
    elif s0 < 0 or s1 < 0:
        return 0

    tot_ways = 0
    for k, v in jump_to_worlds.items():
        if i == 1:
            p0_t = (p0-1-k) % 10 + 1
            s0_t = s0 - p0
            if s0_t < 21:
                tot_ways += v*compute(p0_t, p1, s0_t, s1, 0)
        else:
            p1_t = (p1-1-k) % 10 + 1
            s1_t = s1 - p1
            if s1_t < 21:
                tot_ways += v*compute(p0, p1_t, s0, s1_t, 1)
    ways[(p0, p1, s0, s1, i)] = tot_ways
    return tot_ways

# 7, 10 start
# try all positions!
print(compute(1, 10, 1, 0, 1))  # expect 3
print(compute(1, 3, 1, 3, 0))  # expect 3
print(compute(1, 8, 1, 3, 0))  # expect 0


total_p0_wins = 0
total_p1_wins = 0

for p0 in tqdm.tqdm(range(1, 11)):
    for p1 in tqdm.tqdm(range(1, 11)):
        for s0 in range(21, 31):
            for s1 in range(21):
                total_p0_wins += compute(p0, p1, s0, s1, 1) 
        for s1 in range(21, 31):
            for s0 in range(21):
                total_p1_wins += compute(p0, p1, s0, s1, 0)

print(total_p0_wins)
print(total_p1_wins)
print(total_p0_wins - total_p1_wins)

print()
print(len(ways))
