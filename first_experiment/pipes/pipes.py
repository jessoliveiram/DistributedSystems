import os
import time
from math import ceil
from random import randrange


def random():
    return 1 + randrange(100)


def number_genarator(n):
    sum_n = n + random()
    print("Random number: " + str(sum_n))
    return sum_n


def is_prime(n):
    sqrt_n = n**(1/2)
    for i in range(2, ceil(sqrt_n)):
        if n % i == 0:
            return False
    return True


def write_numbers(write_fd, quantity):
    n = 1
    with open(write_fd, "w", 1) as f:
        for i in range(quantity):
            n = number_genarator(n)
            f.write(str(n)+"\n")
            print("Sent random number")
            time.sleep(1)
        f.write(str(0) + "\n")
        print("End")


def read_numbers(read_fd):
    with open(read_fd, "r") as f:
        while True:
            n = f.read(1)
            print("Number: " + n)
            # if n == 0:
            #     break
            # if is_prime(int(n)):
            #     return "Prime number"
            # return "Not prime"


def parent_child(quantity):
    read_fd, write_fd = os.pipe()
    pid = os.fork()
    if pid > 0:
        print("Init parent process")
        write_numbers(write_fd, quantity)
    else:
        print("Init child process")
        read_numbers(read_fd)
        

def main():
    quantity = 10
    parent_child(quantity)


if __name__ == "__main__":
    main()
