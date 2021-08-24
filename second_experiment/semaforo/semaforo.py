import _thread
import time
import argparse
import threading
from utils.random_number import random
from utils.prime import is_prime


class SemaphoreBuffer:
    TAM_BUFFER = 10
    mutex = threading.Semaphore(1)
    buffer = []
    count = 0

    def insert(self, item):
        self.mutex.acquire()
        if len(self.buffer) < self.TAM_BUFFER:
            self.buffer.append(item)
            print("Produtor produziu:", item)
        self.mutex.release()

    @property
    def remove(self):
        self.mutex.acquire()
        if len(self.buffer) > 0:
            if self.count < 100:
                item = self.buffer.pop(0)
                prime = is_prime(item)
                self.count += 1
                if prime:
                    prime = "É primo"
                else:
                    prime = "Não é primo"
                print("Consumidor consumiu:", item, prime)
            else:
                exit()
        self.mutex.release()
        return self.count


b = SemaphoreBuffer()


def produtor():
    while True:
        item = random(10000000)
        b.insert(item)


def consumidor():
    while True:
        b.remove


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
