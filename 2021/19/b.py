from ast import literal_eval
from collections import defaultdict, Counter, deque
from copy import copy, deepcopy
from dataclasses import dataclass
from functools import reduce
from itertools import chain, permutations, combinations, product
from operator import add, mul
from scipy.spatial.transform import Rotation as R
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

beacons = defaultdict(list)
scanner_id = 0

for line in lines:
    scanner_re = re.match(r"--- scanner (\d+) ---", line)
    if scanner_re:
        scanner_id = int(scanner_re.group(1))
    elif line:
        x, y, z = map(int, line.split(','))
        beacons[scanner_id].append(np.array([x, y, z], dtype=int))
    else:
        continue

# (x, y, z, w)
rot_qats_x = [
    [0, 0, 0, 1],
    [1, 0, 0, 1],
    [1, 0, 0, 0],
    [-1, 0, 0, 1],
]
rot_qats_y = [
    [0, 0, 0, 1],
    [0, 1, 0, 1],
    [0, 1, 0, 0],
    [0, -1, 0, 1],
]
rot_qats_z = [
    [0, 0, 0, 1],
    [0, 0, 1, 1],
    [0, 0, 1, 0],
    [0, 0, -1, 1],
]

rot_mats_x = list(map(lambda q: np.around(R.from_quat(q).as_matrix()).astype(int), rot_qats_x))
rot_mats_y = list(map(lambda q: np.around(R.from_quat(q).as_matrix()).astype(int), rot_qats_y))
rot_mats_z = list(map(lambda q: np.around(R.from_quat(q).as_matrix()).astype(int), rot_qats_z))

combos = list(product(rot_mats_x, rot_mats_y, rot_mats_z))
rot_products = set(map(lambda tup: tuple(tuple(row) for row in (tup[0] @ tup[1] @ tup[2]).tolist()), combos))
print(len(rot_products))

rot_mats = list(map(lambda tup: np.array(tup, dtype=int), rot_products))

# for each scanner, try all 24 rotations and intersect it with another.
# then remap and repeat

np_to_set = lambda arrs: set(tuple(pt.tolist()) for pt in arrs)

# k: sid
# v: list of sets, where each set is a rotation of the original points
rotated_dict = {}

ref_id = 0
ref_set = np_to_set(beacons[0])
for sid, pts in beacons.items():
    if sid == 0:
        continue
        
    rotations = list(map(lambda mat: map(lambda pt: mat @ pt, pts), rot_mats))
    rotated_dict[sid] = list(map(lambda rotation: np_to_set(rotation), rotations))

#req_matches = 3
req_matches = 12

def match(ref_set, cand_set):
    for cand_center in cand_set:
        cx, cy, cz = cand_center
        cand_deltas = set()
        for cand_pt in cand_set:
            x, y, z = cand_pt
            dx, dy, dz = cx-x, cy-y, cz-z
            cand_deltas.add((dx, dy, dz))

        for ref_center in ref_set:
            rx, ry, rz = ref_center
            matches = 0
            for ref_pt in ref_set:
                x, y, z = ref_pt
                dx, dy, dz = rx-x, ry-y, rz-z
                if (dx, dy, dz) in cand_deltas:
                    matches += 1
                    if matches == req_matches:
                        add_tups = lambda tups: tuple(a + b for (a, b) in zip(*tups))
                        shift = add_tups(((rx, ry, rz), (-cx, -cy, -cz)))
                        aligned_cand_pts = set(map(lambda pt: add_tups((pt, shift)), cand_set))
                        return True, aligned_cand_pts, shift
    return False, None, None


match_order = []
to_del = []
while len(rotated_dict) > 0:
    for cand_id, cand_sets in tqdm.tqdm(rotated_dict.items()):
        for cand_set in cand_sets:
            matched, aligned_set, shift = match(ref_set, cand_set)
            if matched:
                ref_set = ref_set | aligned_set
                match_order.append((cand_id, tuple(-x for x in shift)))
                to_del.append(cand_id)
    if to_del:
        for cand_id in to_del:
            del rotated_dict[cand_id]
        to_del = []
    else:
        print(match_order)
        raise ValueError(f'no progress {len(rotated_dict)}')


print(match_order)
print(ref_set)
print(len(ref_set))

scanner_positions = [(0, 0, 0)]
for sid, spos in match_order:
    scanner_positions.append(spos)

print(scanner_positions)

def manhattan(a, b):
    x1, y1, z1 = a
    x2, y2, z2 = b
    return abs(x1-x2) + abs(y1-y2) + abs(z1-z2)

max_hattan = 0
for a, b in combinations(scanner_positions, 2):
    max_hattan = max(max_hattan, manhattan(a, b))

print(max_hattan)
