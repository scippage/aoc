from collections import Counter
from copy import copy

ctr = Counter()
nums = []

with open('input.txt') as f:
    for line in f:
        nums.append(line)

nums2 = copy(nums)

idx = 0
while idx < len(nums[0]):
    for num in nums:
        ctr[idx] += int(num[idx])

    thresh = len(nums)//2 + 1 if len(nums) % 2 == 1 else len(nums)//2
    o2_flag = '1' if ctr[idx] >= thresh else '0'
    
    if len(nums) < 6:
        print(idx, nums, o2_flag, ctr[idx])

    nums = [num for num in nums if num[idx] == o2_flag]
    if len(nums) == 1:
        o2 = nums[0]
        break
    if len(nums) == 0:
        raise ValueError(f"o2 {idx} {o2_flag}")
    idx += 1

print(f"o2 {len(nums)} {idx}")


idx = 0
ctr = Counter()
nums = nums2
while idx < len(nums[0]):
    for num in nums:
        ctr[idx] += int(num[idx])
    co2_flag = '1' if ctr[idx] < len(nums)//2 else '0'
   
    if len(nums) < 6:
        print(idx, nums, co2_flag, ctr[idx])
   
    nums = [num for num in nums if num[idx] == co2_flag]
    if len(nums) == 1:
        co2 = nums[0]
        break
    if len(nums) == 0:
        raise ValueError(f"co2 {idx} {co2_flag}")
    idx += 1

o2 = int(o2, 2)
co2 = int(co2, 2)
print(f"co2 {len(nums)} {idx}")
print(f"{o2}, {co2}, {o2*co2}")

