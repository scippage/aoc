depth = 0
horiz = 0

with open('input.txt') as f:
    for line in f:
        direc, mag = line.split(' ')
        mag = int(mag)
        if direc == 'forward':
            horiz += mag
        elif direc == 'up':
            depth -= mag
        elif direc == 'down':
            depth += mag
        else:
            raise ValueError(line)

print(f"{horiz} {depth} {horiz*depth}")
