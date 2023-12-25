prev_depth = None
ct = 0

with open('input.txt') as f:
    for line in f:
        depth = int(line)
        if prev_depth and depth > prev_depth:
            ct += 1
        prev_depth = depth

print(ct)
