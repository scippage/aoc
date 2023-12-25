from collections import Counter
from functools import reduce

rows = 0
ctr = Counter()

with open('input.txt') as f:
    for line in f:
        for idx, c in enumerate(line.strip()):
            ctr[idx] += int(c)
        rows += 1

gamma = reduce(lambda acc, x: (acc << 1) + x, [0b1 if ct > rows//2 else 0b0 for _, ct in sorted(ctr.items())])
mask = (1 << len(ctr)) - 1
epsilon = ~gamma & mask

print(f"{gamma}, {epsilon}, {gamma*epsilon}")
print(f"{bin(gamma)}, {bin(epsilon)}")

