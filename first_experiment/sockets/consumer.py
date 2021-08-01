import socket

def is_prime_number(n):
    for i in range(2, n):
        if n % i == 0:
            return False
    
    return True



if __name__ == '__main__':
    print(is_prime_number(3))
    print(is_prime_number(16))
    print(is_prime_number(17))
    tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    origin = ('127.0.0.1', 5000)
    tcp.bind(origin)
    tcp.listen(1)
    while True:
        print('Waitting connection...')
        con, client = tcp.accept()
        print('Connect with:', client)
        con.close()