#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <math.h>
#include "utils.h"

using namespace std;
class Semaphore {
public:
    Semaphore(int count_ = 0)  : count(count_) {}

    inline void acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        while (count == 0)
            cv.wait(lock);
        count--;
    }

    inline void release() {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
};


class MutexCounter
{
public:
  MutexCounter()
  {
    this->counter = 0;
  }

  int getCount()
  {
    mutex.lock();
    int val = this->counter;
    mutex.unlock();
    return val;
  }

  void incrementCount()
  {
    this->counter++;

  }

private:
  std::mutex mutex;
  int counter;
};


int main(int argc, char **argv)
{
  if (argc != 4) 
  {
    std::cerr << "Argumentos inválidos." << std::endl;
    std::cout << "Usage: ./main <número de produtores> <número de consumidores> <tamanho do buffer>" << std::endl;
    return -1;
  }

  srand(time(NULL));

  int np = atoi(argv[1]);
  int nc = atoi(argv[2]);
  int buffer = atoi(argv[3]);
  int n = (int)100;

  std::vector<int> data;
  std::vector<std::thread> producer;
  std::vector<std::thread> consumer;

  Semaphore mutex(1);
  Semaphore empty(buffer);
  Semaphore full(0);

  MutexCounter consumer_counter;

  for (unsigned int i = 0; i < np; i++)
  {
    producer.push_back(std::thread([&]() {
      while (true) {
        empty.acquire();
        mutex.acquire();
        data.push_back(generateN());
        mutex.release();
        full.release();
      }
    }));
  }

  for (unsigned int i = 0; i < nc; i++)
  {
    consumer.push_back(std::thread([&]() {
      while (consumer_counter.getCount() < n) {
        consumer_counter.incrementCount();
        full.acquire();
        mutex.acquire();
        int num = data.back();
        data.pop_back();
        std::cout << "Número recebido:" << num << " - " << isPrime(num) << std::endl;
        mutex.release();
        empty.release();
      }
    }));
  }

  for (unsigned int i = 0; i < nc; i++)
    consumer[i].join();
  return 0;
}

int generateN() {
  return 1 + (rand() % (int)10e7);
}

bool isPrime(int n) {
  for (int i = 2; i < sqrt(n); i++)
    if ((n % i) == 0)
      return false;
  return true;
}
