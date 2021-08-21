from math import ceil

def is_prime(num):
    sqrt_num = num ** (1 / 2)
    for i in range(2, ceil(sqrt_num)):
        if num % i == 0:
            return False
    return True
