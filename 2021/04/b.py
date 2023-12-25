import collections
import numpy as np

with open('input.txt') as f:
    lines = f.readlines()

lines = [line.strip() for line in lines]
draws = list(map(lambda x: int(x), lines[0].strip().split(',')))

idx = 1
arrs = []
while idx < len(lines):
    line = lines[idx]
    if not line:
        idx += 1
        arr = []
        for i in range(5):
            row = lines[idx+i]
            row = [int(x) for x in row.split()]
            arr.append(row)
        arr = np.array(arr)
        arrs.append(arr)
        idx += 5

# map number to (board_idx, row_idx, col_idx) key
# map key to value ct

d = collections.defaultdict(list)
cts = collections.defaultdict(int)
arr_sums = [np.sum(arr) for arr in arrs]
arr_draws = collections.defaultdict(set)
not_won = set(range(len(arrs)))

for idx, arr in enumerate(arrs):
    for i in range(5):
        row = arr[i]
        col = arr[:, i]
        for elm in row:
            d[elm].append((idx, i, -1))
        for elm in col:
            d[elm].append((idx, -1, i))

try:
    for draw in draws:
        for key in d[draw]:
            cts[key] += 1
            arr_idx, row_idx, _ = key
            if draw not in arr_draws[arr_idx]:
                arr_sums[arr_idx] -= draw
                arr_draws[arr_idx].add(draw)
            if cts[key] == 5:
                if arr_idx in not_won:
                    not_won.remove(arr_idx)
                if len(not_won) == 0:
                    score = arr_sums[arr_idx]
                    print(f"{key}")
                    print(f"{score} {draw} {draw*score}")
                    raise ValueError
except ValueError:
    print(arrs[arr_idx])

from itertools import chain
n1 = sorted(chain.from_iterable(arrs[arr_idx].tolist()))
print(n1)

for didx, dr in enumerate(draws):
    if dr == draw:
        break

n2 = sorted(draws[:didx+1])
print(n2)

diff = set(n1) - set(n2)
print(diff)
print(sum(diff))
