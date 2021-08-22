import _thread
import argparse
import sys
import time
import threading
from utils.random_number import random


class BufferLimitado:
    TAM_BUFFER = 5
    mutex = threading.Semaphore(1)
    empty = threading.Semaphore(TAM_BUFFER)
    full = threading.Semaphore(0)
    buffer = [None]*TAM_BUFFER
    cheio = 0
    livre = 0

    def insert(self, item):
        self.empty.acquire()
        self.mutex.acquire()
        self.buffer[self.livre] = item
        self.livre = (self.livre + 1) % self.TAM_BUFFER
        self.mutex.release()
        self.full.release()

    def remove(self):
        self.full.acquire()
        self.mutex.acquire()
        item = self.buffer[self.cheio]
        self.cheio = (self.cheio + 1) % self.TAM_BUFFER
        self.mutex.release()
        self.empty.release()
        return item


b = BufferLimitado()


def produtor():
    while True:
        time.sleep(1)
        item = random()
        b.insert(item)
        print("Produtor produziu:", item, b.livre, b.cheio)


def consumidor():
    while True:
        time.sleep(1)
        item = b.remove()
        print("Consumidor consumiu:", item, b.livre, b.cheio)


def main(np, nc):
    for i in range(np):
        _thread.start_new_thread(produtor, ())
    for i in range(nc):
        _thread.start_new_thread(consumidor, ())
    while 1:
        pass


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("-np", "--nprodutor", required=True, nargs=1)
    ap.add_argument("-nc", "--nconsumidor", required=True, nargs=1)
    args = vars(ap.parse_args())
    main(int(args['nprodutor'][0]), int(args['nconsumidor'][0]))
