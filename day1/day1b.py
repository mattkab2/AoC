
data = []
with open('input.txt','r') as inp:
    for line in inp:
        data.append(int(line))

ret = 0
for i in range(1,len(data)-2):
    lavg = data[i-1]+data[i]+data[i+1]
    ravg = data[i]+data[i+1]+data[i+2]
    if (ravg > lavg): ret += 1

print(ret)
