import socket


def is_prime_number(n):
    for i in range(2, n):
        if n % i == 0:
            return str(n) + ' is not prime.'
    return str(n) + ' is prime.'


if __name__ == '__main__':
    tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    origin = ('127.0.0.1', 5000)
    tcp.bind(origin)
    tcp.listen(1)
    while True:
        print('Waiting connection...')
        con, client = tcp.accept()
        print('Connected with:', client)
        number = int(con.recv(1024).decode())

        print('Receive:', number)
        if number == 0:
            message = 'Job done! Exiting...'
            print(message)
            con.send(message.encode())
            con.close()
            exit()
        message = is_prime_number(number)
        print('Sending: ', message)
        con.send(message.encode())
        con.close()
