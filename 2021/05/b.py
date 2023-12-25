from itertools import chain
import numpy as np

with open('input.txt') as f:
    lines = f.readlines()

segs = []

for line in lines:
    pt1, pt2 = line.split('->')
    pt1 = pt1.strip()
    pt2 = pt2.strip()

    x1, y1 = map(lambda x: int(x), pt1.split(','))
    x2, y2 = map(lambda x: int(x), pt2.split(','))
    
    segs.append(((x1, y1), (x2, y2)))

pts = list(chain.from_iterable(segs))

max_x = max(x for (x, y) in pts) + 1
max_y = max(y for (x, y) in pts) + 1
print(f"{max_x}, {max_y}")

arr = np.zeros((max_x, max_y))

for pt1, pt2 in segs:
    x1, y1 = pt1
    x2, y2 = pt2
    if x1 == x2:
        range_args = (y1, y2+1) if y1 <= y2 else (y1, y2-1, -1)
        for y in range(*range_args):
            arr[x1][y] += 1
    elif y1 == y2:
        range_args = (x1, x2+1) if x1 <= x2 else (x1, x2-1, -1)
        for x in range(*range_args):
            arr[x][y1] += 1
    else:
        range_args_x = (x1, x2+1) if x1 <= x2 else (x1, x2-1, -1)
        range_args_y = (y1, y2+1) if y1 <= y2 else (y1, y2-1, -1)

        for x, y in zip(range(*range_args_x), range(*range_args_y)):
            arr[x][y] += 1

print(max_x*max_y)
print(np.sum(arr == 0))
print(np.sum(arr == 1))
print(np.sum(arr >= 2))
