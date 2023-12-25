from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from functools import reduce
from itertools import chain
import math
import numpy as np
import sys

if len(sys.argv) == 1:
    filename = 'input.txt'
elif sys.argv[1] == 'a':
    filename = 'test_a.txt'
elif sys.argv[1] == 'b':
    filename = 'test_b.txt'
else:
    raise ValueError()
print(filename)

with open(filename) as f:
    lines = f.readlines()
    lines = [line.strip() for line in lines]


class Node:
    def __init__(self, pt):
        self.pt = pt
        self.neighbors = set()

    def is_small(self):
        return self.pt.islower()

    def __repr__(self):
        return f'{self.pt}, {"|".join([p.pt for p in self.neighbors])}'

    def __str__(self):
        return self.pt

nodes = defaultdict(lambda: Node(None))

for line in lines:
    src, dst = line.split('-')
    nodes[src].pt = src
    nodes[dst].pt = dst
    nodes[src].neighbors.add(nodes[dst])
    nodes[dst].neighbors.add(nodes[src])

print(nodes)

paths = set()
def search(curr, end, path, visited):
    if curr in visited:
        return
    if curr == end:
        paths.add('-'.join((str(p) for p in path)))
        return
    if curr.is_small():
        visited.add(curr)

    for neighbor in curr.neighbors:
        search(neighbor, end, path + [neighbor], copy(visited))

search(nodes['start'], nodes['end'], [nodes['start']], set())

print(paths)
print(len(paths))
