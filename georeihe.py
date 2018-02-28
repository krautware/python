def georeihe(b, e):
    e *= -1
    return 1/(1-b**e)

def geo_addition(b, e):
    res = 1

    for x in range(1, 101):
        res += b**(e*(-x))
    return res

def prim():
    
    for x in range(2, 1000):
        is_prime = True
        for y in range(2, x):
            if x%y == 0:
                is_prime = False
                break
        if is_prime:
            yield x


prime = prim()

for x in range(1,11):
    p = next(prime)
    print("x = {}, p = {}: {} ---> {}".format(x, p, georeihe(p, x), geo_addition(p, x)))
