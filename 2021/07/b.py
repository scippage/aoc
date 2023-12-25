from collections import defaultdict, Counter
from itertools import chain
import math
import numpy as np

with open('input.txt') as f:
    lines = f.readlines()

line = lines[0].strip()
nums = [int(num) for num in line.split(',')]

# original solution just did a brute force search
# optimization function is the average of L1 and L2, so it should be between the mean and median
# this solution should be wrong in general
# edit: take df/dx and set the convex fn to 0. you see that the optimum is the mean plus some error term with abs(x) < 0.5, so try rounding both ways. no closed form known.

mean_int = math.floor(np.mean(nums))
print(mean_int)
gas = 0
arith_sum = lambda x: x*(x+1)//2

for num in nums:
    gas += arith_sum(abs(num-mean_int))

print(gas)

gas = float('inf')
best_cand = None
for candidate in range(max(nums)):
    s = 0
    for num in nums:
        s += arith_sum(abs(num-candidate))
    if s <= gas:
        gas = s
        best_cand = candidate
    #print(candidate, s)
    
print(best_cand)
print(gas)
