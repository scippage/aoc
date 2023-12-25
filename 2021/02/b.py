depth = 0
horiz = 0
aim = 0

with open('input.txt') as f:
    for line in f:
        direc, mag = line.split(' ')
        mag = int(mag)
        if direc == 'forward':
            horiz += mag
            depth += aim*mag
        elif direc == 'up':
            aim -= mag
        elif direc == 'down':
            aim += mag
        else:
            raise ValueError(line)

print(f"{horiz} {depth} {aim} {horiz*depth}")
