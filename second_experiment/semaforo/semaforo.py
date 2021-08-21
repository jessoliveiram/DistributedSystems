import _thread
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


_thread.start_new_thread(produtor, ())
_thread.start_new_thread(consumidor, ())

while 1:
    pass
