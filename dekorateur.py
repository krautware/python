def is_prime(n):
    return all(n % i for i in range(2,n))

