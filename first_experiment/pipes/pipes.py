import os
import sys
import time
from math import ceil
from random import randrange


def random():
    return 1 + randrange(100)


def generate_number(n):
    sum_n = n + random()
    print("Random number: " + str(sum_n))
    return sum_n


def write_numbers(write_fd, quantity):
    num = 1
    with open(write_fd, "w", 1) as f:
        for i in range(quantity):
            num = generate_number(num)
            f.write(str(num) + "\n")
            print("Sending random number")
            time.sleep(1)
        f.write(str(0) + "\n")
        print("Sending zero")


def increment_value(s, num):
    s += num
    return s


def prime(num):
    sqrt_num = num ** (1 / 2)
    for i in range(2, ceil(sqrt_num)):
        if num % i == 0:
            return False
    return True


def evaluate_number(num):
    if num == 0:
        print("End")
    elif prime(num):
        print("Prime number")
    else:
        print("Not prime")


def read_numbers(read_fd):
    with open(read_fd, "r") as f:
        s = ""
        while True:
            num = f.read(1)
            if num != "\n":
                s = increment_value(s, num)
            else:
                evaluate_number(int(s))
                s = ""


def parent_child(quantity):
    read_fd, write_fd = os.pipe()
    pid = os.fork()
    if pid > 0:
        print("Init parent process")
        write_numbers(write_fd, quantity)
    else:
        print("Init child process")
        read_numbers(read_fd)
        

def main(n):
    parent_child(n)


if __name__ == "__main__":
    args = sys.argv
    if len(args) == 1:
        print('Inform the quantity of numbers')
    main(int(args[1]))
