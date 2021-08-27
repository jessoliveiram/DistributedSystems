#include <vector>
#include <atomic>
#include <iostream>
#include <thread>
#include <numeric>
#include <chrono>

using namespace std;
//global vars
atomic_flag lock_sum = ATOMIC_FLAG_INIT;
int total = 0;

//function to generate a vector with n random numbers between -100 and 100
//the numbers are representing with 1 byte char
vector<signed char> generate_numbers_vector(int n)
{
    vector<signed char> numbers_vector;
    srand(time(NULL));
    for (int i =0; i< n; i++)
    {
        int rand_number = (rand() % 201) -100;
        numbers_vector.push_back(rand_number);
    }

    return numbers_vector;
}


//function to sum all elements of a vector and write the result in a global var
void thread_safe_sum_vector_elements(vector<signed char>::const_iterator begin, vector<signed char>::const_iterator end)
{
    int sub_total = accumulate(begin, end, 0);
    while(lock_sum.test_and_set()){}
    total += sub_total;
    lock_sum.clear();
}

//Divide a vector of numbers in n threads to realize the sum of elements
void parallel_sum(vector<signed char> numbers, int n_threads)
{
    int thread_piece = (int) numbers.size()/n_threads;
    vector<thread> threads;
    for(int i=0; i<n_threads; i++)
    {
        vector<int> sub_vector = {numbers.begin() + (i * thread_piece), numbers.begin() + (i * thread_piece)+thread_piece};
        if(i== n_threads -1){
            sub_vector = {numbers.begin() +i * thread_piece, numbers.end()};
        }

        threads.emplace_back(thread_safe_sum_vector_elements, 
                             numbers.begin() + i * (thread_piece), 
                             numbers.end() -(n_threads-i-1)*(thread_piece));
    }
    for(auto& th : threads)
    {
        th.join();
    }

}

//runs the sum of N numbers with K threads
void generate_case_study(int N, int K)
{
    vector<signed char> numbers = generate_numbers_vector(N);
    double total_duration = 0;
    for (int i = 0; i < 10; i++)
    {
        total = 0;
        auto start = chrono::system_clock::now();
        parallel_sum(numbers, K);
        auto end = chrono::system_clock::now();
        chrono::duration<double> duration = end - start;
        total_duration += duration.count();
    }
    cout << "N = " << N << endl;
    cout << "K = " << K << endl;
    cout << total_duration/10 << endl;
}

//The main function executes the case study with different values
int main()
{

    int n_threads = 1;
    while (n_threads <= 256)
    {
        generate_case_study(10000000, n_threads);
        generate_case_study(100000000, n_threads);
        generate_case_study(1000000000, n_threads);
        n_threads = n_threads * 2;
    }
    
    
}

