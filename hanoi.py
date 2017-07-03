def hanoi(scheiben, start, hilfe, ziel):
    def start_hanoi(scheiben, start, hilfe, ziel):
        if scheiben > 1:
            yield from start_hanoi(scheiben - 1, start, ziel, hilfe)
            yield start, ziel
            yield from start_hanoi(scheiben - 1, hilfe, start, ziel)
        else:
            yield start, ziel
    yield from start_hanoi(scheiben, start, hilfe, ziel)    
    yield start, start

v = []
v.append([i for i in range(5,0,-1)])
v.append([]), v.append([])

f = hanoi(5, 0, 1, 2)

s, z = next(f)

while s != z:
    print("{} -> {}".format(s,z))
    v[z].append(v[s].pop())
    print(v)
    s, z = next(f)
