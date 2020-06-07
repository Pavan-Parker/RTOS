n=[]
for line in reversed(list(open("P2P/ports.txt"))):
    n.append(line.rstrip())
print(n)