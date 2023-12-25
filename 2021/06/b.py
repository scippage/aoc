import collections

with open('input.txt') as f:
    lines = f.readlines()

line = lines[0].strip()
nums = [int(num) for num in line.split(',')]
ctr = collections.Counter(nums)

for i in range(256):
    new_ctr = collections.Counter()
    for k, v in sorted(ctr.items()):
        if k == 0:
            new_ctr[8] += v
            new_ctr[6] += v
        else:
            new_ctr[k-1] += v
    ctr = new_ctr

print(ctr)
print(sum(v for v in ctr.values()))
