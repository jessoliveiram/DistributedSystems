import os
from multiprocessing import Process
from timeit import default_timer as timer

from third_experiment.process.evaluator import verify_result


def run_process(REPEAT, PORT, SLEEP):
    os.system(f"./a.out {REPEAT} {PORT} {SLEEP}")


def run_n_process(N, REPEAT, SLEEP):
    PORT = 56823
    for i in range(N):
        start = timer()
        Process(target=run_process, args=(REPEAT, PORT, SLEEP)).start()
        stop = timer()
        print("N = " + str(i) + " Time: " + str(stop - start))
        PORT += 1


def study_case_basic():
    print("Case Basic")
    start = timer()
    run_n_process(2, 10, 2)
    stop = timer()
    print("N = " + "2" + " Time: " + str(stop - start))
    verify_result(2, 10)


def study_case_test_1():
    print("Case 1")
    for i in [2, 4, 8, 16, 32]:
        start = timer()
        run_n_process(i, 10, 2)
        stop = timer()
        print("N = " + str(i) + " Time: " + str(stop - start))
        verify_result(i, 10)


def study_case_test_2():
    print("Case 2")
    for i in [2, 4, 8, 16, 32, 64]:
        start = timer()
        run_n_process(i, 5, 1)
        stop = timer()
        print("N = " + str(i) + " Time: " + str(stop - start))
        verify_result(i, 5)


def study_case_test_3():
    print("Case 2")
    for i in [2, 4, 8, 16, 32, 64, 128]:
        start = timer()
        run_n_process(i, 3, 0)
        stop = timer()
        print("N = " + str(i) + " Time: " + str(stop - start))
        verify_result(i, 3)


if __name__ == '__main__':
    study_case_basic()
    study_case_test_1()
    study_case_test_2()
    study_case_test_3()
