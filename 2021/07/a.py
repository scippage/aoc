from collections import defaultdict, Counter
from itertools import chain
import math
import numpy as np

with open('input.txt') as f:
    lines = f.readlines()

line = lines[0].strip()
nums = [int(num) for num in line.split(',')]

median_int = list(sorted(nums))[len(nums)//2 - 1]
print(median_int)
gas = 0

for num in nums:
    gas += abs(num-median_int)

print(gas)
