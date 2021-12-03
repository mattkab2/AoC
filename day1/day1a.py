
data = []
with open('input.txt','r') as inp:
    for line in inp:
        data.append(int(line))

ret = 0
for i in range(1,len(data)):
    if (data[i] > data[i-1]): ret += 1

print(ret)
