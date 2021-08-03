import random
import socket
import sys


def generate_number():
    return random.randrange(100) + 1


def send_numbers(quantity):
    n = 1
    for i in range(quantity):
        n = n + generate_number()
        print('Sending: ', n)
        send_number(n)
    print('Sending: ', 0)
    send_number(0)


def send_number(n):
    tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    destiny = ('127.0.0.1', 5000)
    tcp.connect(destiny)
    tcp.send(str(n).encode())
    response = tcp.recv(1024)

    print('Receive: ', response.decode())
    tcp.close()


if __name__ == '__main__':
    args = sys.argv
    if len(args) == 1:
        print('Inform the quantity of numbers')
    send_numbers(int(args[1]))
