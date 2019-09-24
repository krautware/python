def laplace(a):
    count = 0
    hit = 0
    a = a % 10

    for i in range(10):
        if i == a:
            hit += 1
        for j in range(10):
            if j == a:
                hit += 1
            for k in range(10):
                if k == a:
                    hit += 1
                for l in range(10):
                    if l == a:
                        hit += 1
                    if hit == 2:
                        count += 1
                    if l == a:
                        hit -= 1
                if k == a:
                    hit -= 1
            if j == a:
                hit -= 1
        if i == a:
            hit -= 1
    return count

print(laplace(3))