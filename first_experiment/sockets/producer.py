import random, socket, sys


def generate_number():
    return random.randrange(100) + 1

def send_numbers(quantity):
    n = 1
    for i in range(quantity):
        n = n + generate_number()
        send_number(n)
        print(n)        

def send_number(n):
    tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    destiny = ('127.0.0.1', 5000)
    tcp.connect(destiny)
    tcp.close

if __name__ == '__main__':
    args = sys.argv
    send_numbers(int(args[1]))
    