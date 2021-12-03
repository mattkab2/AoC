
depth = 0
horz = 0
aim = 0
with open('input.txt','r') as inp:
    for line in inp:
        instr,step = line.split()
        step = int(step)
        if instr=="down":
            aim += step
        elif instr=="up":
            aim -= step
        elif instr=="forward":
            horz += step
            depth += step*aim

print(depth*horz)

