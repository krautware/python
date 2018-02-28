zeta = 0
diff = 0
for x in range(1,100000):
    zeta += 1/(x*3)
    if x%100 == 0:
        print(zeta-diff)
        diff = zeta

print(zeta)