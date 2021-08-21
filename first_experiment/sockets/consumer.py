import socket
from utils.prime import is_prime


def listen_tcp():
    tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    origin = ('127.0.0.1', 5000)
    tcp.bind(origin)
    tcp.listen(1)
    n = 1
    while n != 0:
        print('Waiting connection...')
        connection = receive_connection(tcp)
        n = int(connection.recv(1024).decode())
        message = process_message(n)
        print('Sending: ', message)
        connection.send(message.encode())
        connection.close()
    tcp.close()


def receive_connection(tcp_socket):
    connection, client = tcp_socket.accept()
    print('Connected with:', client)
    return connection


def process_message(number):
    print('Receive:', number)
    if number != 0:
        if is_prime(number):
            return str(number) + ' is prime.'
        else: 
            return str(number) + ' is not prime.'
    return 'Job is done! Exiting...'


if __name__ == '__main__':
    listen_tcp()
