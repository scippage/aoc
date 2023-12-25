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

# use test_a.txt for input
rows = []
pattern = '.*?#([A-D])#([A-D])#([A-D])#([A-D])#.*?'
for line in lines[2:6]:
    match = re.match(pattern, line)
    r1 = match.group(1)
    r2 = match.group(2)
    r3 = match.group(3)
    r4 = match.group(4)
    rows.append((r1, r2, r3, r4))

amphipods = {
    'A': 0,
    'B': 1,
    'C': 2,
    'D': 3,
}
amphipods_rev = {v:k for k,v in amphipods.items()}

costs = {
    'A': 1,
    'B': 10,
    'C': 100,
    'D': 1000,
}

# (hall_idx -> room_idx) dists
hdists = {
    (0, 0): 2,
    (1, 0): 1,
    (2, 0): 1,
    (3, 0): 3,
    (4, 0): 5,
    (5, 0): 7,
    (6, 0): 8,
    (0, 1): 4,
    (1, 1): 3,
    (2, 1): 1,
    (3, 1): 1,
    (4, 1): 3,
    (5, 1): 5,
    (6, 1): 6,
    (0, 2): 6,
    (1, 2): 5,
    (2, 2): 3,
    (3, 2): 1,
    (4, 2): 1,
    (5, 2): 3,
    (6, 2): 4,
    (0, 3): 8,
    (1, 3): 7,
    (2, 3): 5,
    (3, 3): 3,
    (4, 3): 1,
    (5, 3): 1,
    (6, 3): 2,
}

'''
#############
#...........#
###D#A#C#A###
  #D#C#B#A#
  #D#B#A#C#
  #D#C#B#B#
  #########
'''

def draw(state):
    hallway, rooms = state
    return f'''
      #############
      #{hallway[0]}{hallway[1]}.{hallway[2]}.{hallway[3]}.{hallway[4]}.{hallway[5]}{hallway[6]}#
      ###{rooms[0][0]}#{rooms[1][0]}#{rooms[2][0]}#{rooms[3][0]}###
        #{rooms[0][1]}#{rooms[1][1]}#{rooms[2][1]}#{rooms[3][1]}#
        #{rooms[0][2]}#{rooms[1][2]}#{rooms[2][2]}#{rooms[3][2]}#
        #{rooms[0][3]}#{rooms[1][3]}#{rooms[2][3]}#{rooms[3][3]}#
        #########
     '''


# if one can move in, aside from size
def is_livable(state, room_idx):
    # rtype: bool
    _, rooms = state
    amphipod = amphipods_rev[room_idx]

    first_amphipod_idx = float('inf')
    for idx, cell in enumerate(rooms[room_idx]):
        if idx > first_amphipod_idx:
            if cell != amphipod:
                return False
        else:
            if cell == amphipod:
                first_amphipod_idx = idx
            elif cell != '.':
                return False
    return True


# keep track of from separately
def move_into(state, room_idx):
    # rtype: rooms, depth
    _, rooms = state
    room = rooms[room_idx]
    amphipod = amphipods_rev[room_idx]
    for idx in range(len(room)-1, -1, -1):
        if room[idx] == '.':
            new_room = list(room)
            new_room[idx] = amphipod
            
            new_rooms = list(rooms)
            new_rooms[room_idx] = tuple(new_room)
            return tuple(new_rooms), idx+1
    raise ValueError(f'no vacany {room_idx}')


# keep track of to separately
def move_out(state, room_idx):
    # rtype: rooms, depth, leaving amphipod
    _, rooms = state
    room = rooms[room_idx]
    for idx, cell in enumerate(room):
        if cell != '.':
            new_room = list(room)
            new_room[idx] = '.'

            new_rooms = list(rooms)
            new_rooms[room_idx] = tuple(new_room)
            return tuple(new_rooms), idx+1, cell
    raise ValueError(f'empty room')


# consider only hallidx [1],2,3,4,[5] as blockers
#...........#
###D#A#C#A###
# from (hall_idx -> room_idx)
def is_blocked(hallway, hall_idx, room_idx):
    if room_idx == 0:
        if (
            (hallway[1]!='.' and hall_idx < 1) or
            (hallway[2]!='.' and hall_idx > 2) or
            ((hallway[2]!='.' or hallway[3]!='.') and hall_idx > 3) or
            ((hallway[2]!='.' or hallway[3]!='.' or hallway[4]!='.') and hall_idx > 4) or
            ((hallway[2]!='.' or hallway[3]!='.' or hallway[4]!='.' or hallway[5]!='.') and hall_idx > 5)
        ):
            return True
    elif room_idx == 1:
        if (
            ((hallway[1]!='.' or hallway[2]!='.') and hall_idx < 1) or
            (hallway[2]!='.' and hall_idx < 2) or
            (hallway[3]!='.' and hall_idx > 3) or
            ((hallway[3]!='.' or hallway[4]!='.') and hall_idx > 4) or
            ((hallway[3]!='.' or hallway[4]!='.' or hallway[5]!='.') and hall_idx > 5)
        ):
            return True
    elif room_idx == 2:
        if (
            ((hallway[1]!='.' or hallway[2]!='.' or hallway[3]!='.') and hall_idx < 1) or
            ((hallway[2]!='.' or hallway[3]!='.') and hall_idx < 2) or
            (hallway[3]!='.' and hall_idx < 3) or
            (hallway[4]!='.' and hall_idx > 4) or
            ((hallway[4]!='.' or hallway[5]!='.') and hall_idx > 5)
        ):
            return True
    elif room_idx == 3:
        if (
            ((hallway[1]!='.' or hallway[2]!='.' or hallway[3]!='.' or hallway[4]!='.') and hall_idx < 1) or
            ((hallway[2]!='.' or hallway[3]!='.' or hallway[4]!='.') and hall_idx < 2) or
            ((hallway[3]!='.' or hallway[4]!='.') and hall_idx < 3) or
            (hallway[4]!='.' and hall_idx < 4) or
            (hallway[5]!='.' and hall_idx > 5)
        ):
            return True
    else:
        raise ValueError(room_idx)
    return False


def neighbors(state):
    # rtype: set of tuples (state, gas)
    hallway, rooms = state
    states_to_try = set()
    
    moveins_to_try = set()
    moveouts_to_try = set()
    for room_idx in range(len(rooms)):
        if is_livable(state, room_idx):
            # add only if not full
            if rooms[room_idx][0] == '.':
                moveins_to_try.add(room_idx)
        else:
            moveouts_to_try.add(room_idx)
    
    # try moves
    for hall_idx, c in enumerate(hallway):
        if c == '.':
            # try moving out
            for room_idx in moveouts_to_try:
                if is_blocked(hallway, hall_idx, room_idx):
                    continue

                hdepth = hdists[(hall_idx, room_idx)]
                new_rooms, vdepth, amphipod = move_out(state, room_idx)
                new_hallway = list(hallway)
                new_hallway[hall_idx] = amphipod
                new_hallway = tuple(new_hallway)
                new_state = (new_hallway, new_rooms)
                gas = (hdepth+vdepth)*costs[amphipod]

                states_to_try.add((new_state, gas))
        else:
            # try moving in
            amphipod_idx = amphipods[c]
            if amphipod_idx in moveins_to_try:
                if is_blocked(hallway, hall_idx, amphipod_idx):
                    continue
                
                hdepth = hdists[(hall_idx, amphipod_idx)]
                new_rooms, vdepth = move_into(state, amphipod_idx)
                new_hallway = list(hallway)
                new_hallway[hall_idx] = '.'
                new_hallway = tuple(new_hallway)
                new_state = (new_hallway, new_rooms)
                gas = (hdepth+vdepth)*costs[c]

                states_to_try.add((new_state, gas))
    return states_to_try


hallway = tuple('.'*7)
rooms = tuple(zip(*rows))  # transposition wizardry
state = (hallway, rooms)
print(state)
print(draw(state))


final_hallway = tuple('.'*7)
final_rooms = tuple(tuple(c)*4 for c in ['A', 'B', 'C', 'D'])
final_state = (final_hallway, final_rooms)
print(final_state)


visited = set()
q = []  # priority, state, path
q.append((0, state, [state]))

final_path = []
final_priority = 0

# dijkstra's without decrease-key
while q:
    priority, state, path = heapq.heappop(q)
    if state in visited:
        continue

    if state == final_state:
        final_path = path
        final_priority = priority
        break

    for neighbor in neighbors(state):
        neighbor_state, gas = neighbor
        heapq.heappush(q, (priority + gas, neighbor_state, path + [neighbor_state]))

    visited.add(state)
else:
    raise ValueError("no end")

for s in final_path:
    print(draw(s))

print(final_priority)
print(len(visited))
