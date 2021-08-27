#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <math.h>


using namespace std;
//Class to representing a counter semaphore
class Semaphore {
public:
    Semaphore(int count_ = 0)  : count(count_) {}

    inline void wait() {
        unique_lock<mutex> lock(mtx);
        while (count == 0)
            cv.wait(lock);
        count--;
    }

    inline void signal() {
        unique_lock<mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

    void set_count(int new_count)
    {
      count = new_count;
    }

    int get_count()
    {
      return count;
    }

private:
    mutex mtx;
    condition_variable cv;
    int count;
};

//Class to representing a counter with a mutex on all operates 
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
    mutex_counter.lock();
    this->counter++;
    mutex_counter.unlock();
  }

  void clear_count()
  {
    this->counter = 0;
  }

private:
  mutex mutex_counter;
  int counter;
};

//Initializing the global vars
vector<int> data;
int n = 10e5;
Semaphore mutex_data(1);
Semaphore empty(1);
Semaphore full(0);

MutexCounter consumer_counter;
MutexCounter producer_counter;

// function to generate a random number between 1 and 10^7
int generate_number() {
  srand(time(NULL));
  return 1 + (rand() % (int)10e7);
}

// function to check if a number is prime
bool is_prime(int n) {
  for (int i = 2; i < sqrt(n); i++)
    if ((n % i) == 0)
      return false;
  return true;
}

//function to consume a number of the shared vector and check if it is prime
void consume()
{
  while(consumer_counter.get_count() < n) {
        consumer_counter.increment_count();
        full.wait();
        mutex_data.wait();
        int num = data.back();
        data.pop_back();
        //cout << "Número recebido:" << num << " - " << is_prime(num) << endl;
        mutex_data.signal();
        empty.signal();
      }
}

//function to insert a random number in the shared vector
void produce()
{
  while(producer_counter.get_count() < n) {
        producer_counter.increment_count();
        empty.wait();
        mutex_data.wait();
        data.push_back(generate_number());
        mutex_data.signal();
        full.signal();
      }
}

//function to execute threads by define in case study 
void generate_case_study(int NP, int NC, int N)
{
  double total_duration = 0;
  empty.set_count(N);
  for(int j = 0; j < 10; j++)
  {
   
    vector<thread> producers;
    vector<thread> consumers;
    auto start = chrono::system_clock::now();
    for (unsigned int i = 0; i < NP; i++)
      producers.emplace_back(produce);
    

    for (unsigned int i = 0; i < NC; i++)
      consumers.emplace_back(consume);

    for (auto& producer : producers)
      producer.join();

    for (auto& consumer : consumers)
      consumer.join();

    auto end = chrono::system_clock::now();
    chrono::duration<double> duration = end - start;
    total_duration += duration.count();
    consumer_counter.clear_count();
    producer_counter.clear_count();   
  }
  cout << "N = " << empty.get_count() << endl;
  cout << "Np = " << NP << endl;
  cout << "Nc = " << NC << endl;
  cout << total_duration/10 << endl;
}

//In the main function runs the case study with different params
int main()
{
  int i = 1;
  int n = 32;
  while(n<=32)
  {
    
    while(i <= 16)
    {
      generate_case_study(i,1,n);
      generate_case_study(1,i,n);
      i = i * 2;
    }
    i = 0;
    n = n*2;
  }

}
