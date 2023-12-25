from collections import deque

q = deque(maxlen=3)
q_sum = 0

ct = 0

with open('input.txt') as f:
    for line in f:
        depth = int(line)
        
        if len(q) < 3:
            q.append(depth)
            q_sum += depth
            continue
        
        prev_sum = q_sum
        q_sum -= q.popleft()
        q.append(depth)
        q_sum += depth

        if prev_sum < q_sum:
            ct += 1

print(ct)
