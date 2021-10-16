import os
import time
import sys
from multiprocessing import Process, process
from timeit import default_timer as timer

from evaluator import verify_result


def run_process(REPEAT, PORT, SLEEP):
    os.system(f"./process.out {REPEAT} {PORT} {SLEEP}")


def run_n_process(N, REPEAT, SLEEP):
    PORT = 56823
    process_list = []
    start = time.time()
    for i in range(N):        
        p = Process(target=run_process, args=(REPEAT, PORT, SLEEP))
        process_list.append(p)
        p.start()     
        PORT += 1
    for p in process_list:
        p.join()
    stop = time.time()
    elapsed_time = stop - start
    print("N = " + str(N) + " Time: " + str(elapsed_time))

def study_case_basic():
    run_n_process(2, 10, 2)
    #verify_result(2, 10)


def study_case_test_1():
    for i in [2, 4, 8, 16, 32]:
        run_n_process(i, 10, 2)
        #verify_result(i, 10)


def study_case_test_2():
    for i in [2, 4, 8, 16, 32, 64]:
        run_n_process(i, 5, 1)
        #verify_result(i, 5)


def study_case_test_3():
    for i in [2, 4, 8, 16, 32, 64, 128]:
        run_n_process(i, 3, 0)
        #verify_result(i, 3)


if __name__ == '__main__':
    if len(sys.argv) > 1:
        if sys.argv[1] == "1":
            study_case_test_1()
        if sys.argv[1] == "2":     
            study_case_test_2()
        if sys.argv[1] == "3":
            study_case_test_3()
    else:
        study_case_basic()
    