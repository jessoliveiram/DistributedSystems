import os
from random import randrange


def random():
    return 1 + randrange(100)


def number_genarator(n):
    sum_n = n + random()
    print("Random number: " + str(sum_n))
    return sum_n


def is_prime(n):
    sqrt_n = n**(1/2)
    for i in range(2, sqrt_n):
        if n % i == 0:
            return "Nao e primo!"
    return "E primo"


def write_numbers(write_fd, quantity):
    n = 1
    write_opened = os.fdopen(write_fd, 'w')
    for i in range(int(quantity)):
        n = number_genarator(n)
        text = str(n) + " "
        
        write_opened.write(text)
    os.write(write_fd, str.encode("0"))
    write_opened.close()


def read_numbers(read_fd):
    n = (os.read(read_fd, 20)).decode()
    print("Number readed: " + n)


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
