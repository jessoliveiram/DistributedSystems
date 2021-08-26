#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <math.h>


using namespace std;

class Semaphore {
public:
    Semaphore(int count_ = 0)  : count(count_) {}

    inline void acquire() {
        unique_lock<mutex> lock(mtx);
        while (count == 0)
            cv.wait(lock);
        count--;
    }

    inline void release() {
        unique_lock<mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

private:
    mutex mtx;
    condition_variable cv;
    int count;
};


class MutexCounter
{
public:
  MutexCounter()
  {
    this->counter = 0;
  }

  int get_count()
  {
    mutex_counter.lock();
    int val = this->counter;
    mutex_counter.unlock();
    return val;
  }

  void increment_count()
  {
    this->counter++;

  }

  void clear_count()
  {
    this->counter = 0;
  }

private:
  mutex mutex_counter;
  int counter;
};


vector<int> data;
int buffer = 100;
int n = 10e5;
Semaphore mutex_data(1);
Semaphore empty(buffer);
Semaphore full(0);

MutexCounter consumer_counter;

int generate_number() {
  srand(time(NULL));
  return 1 + (rand() % (int)10e7);
}

bool is_prime(int n) {
  for (int i = 2; i < sqrt(n); i++)
    if ((n % i) == 0)
      return false;
  return true;
}

void consume()
{
  while(consumer_counter.get_count() < n) {
        consumer_counter.increment_count();
        full.acquire();
        mutex_data.acquire();
        int num = data.back();
        data.pop_back();
        //cout << "Número recebido:" << num << " - " << is_prime(num) << endl;
        mutex_data.release();
        empty.release();
      }
}

void produce()
{
  while(consumer_counter.get_count() < n) {
        empty.acquire();
        mutex_data.acquire();
        data.push_back(generate_number());
        mutex_data.release();
        full.release();
      }
}

void generate_case_study(int NP, int NC)
{
  double total_duration = 0;
  for(int i = 0; i < 10; i++)
  {
   
    vector<thread> producers;
    vector<thread> consumers;
    auto start = chrono::system_clock::now();
    for (unsigned int i = 0; i < NP; i++)
      producers.emplace_back(produce);
    

    for (unsigned int i = 0; i < NC; i++)
      consumers.emplace_back(consume);

    for (auto& consumer : consumers)
      consumer.join();

    for (auto& producer : producers)
      producer.join();

    auto end = chrono::system_clock::now();
    chrono::duration<double> duration = end - start;
    total_duration += duration.count();
    consumer_counter.clear_count();   
  }
  cout << "Np = " << NP << endl;
  cout << "Nc = " << NC << endl;
  cout << total_duration/10 << endl;
}

int main()
{
  int i = 1;
  while(i <= 16)
  {
    generate_case_study(i,1);
    generate_case_study(1,i);
    i = i * 2;
  }
  
}
