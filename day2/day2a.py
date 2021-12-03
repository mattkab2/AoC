
depth = 0
horz = 0
with open('input.txt','r') as inp:
    for line in inp:
        instr,step = line.split()
        step = int(step)
        if instr=="down":
            depth += step
        elif instr=="up":
            depth -= step
        elif instr=="forward":
            horz += step

print(depth*horz)

