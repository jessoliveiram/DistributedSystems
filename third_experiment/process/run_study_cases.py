import os
from multiprocessing import Process


def run_process(REPEAT, PORT, SLEEP):
    os.system(f"./process.out {REPEAT} {PORT} {SLEEP}")


def run_n_process(N, REPEAT, SLEEP):
    PORT = 56823
    for i in range(N):
        Process(target=run_process, args=(REPEAT, PORT, SLEEP)).start()
        PORT += 1


def study_case_basic():
    run_n_process(2, 10, 2)


def study_case_test_1():
    for i in [2, 4, 8, 16, 32]:
        run_n_process(i, 10, 2)


def study_case_test_2():
    for i in [2, 4, 8, 16, 32, 64]:
        run_n_process(i, 5, 1)


def study_case_test_3():
    for i in [2, 4, 8, 16, 32, 64, 128]:
        run_n_process(i, 3, 0)


if __name__ == '__main__':
    study_case_basic()
    # study_case_test_1()
    # study_case_test_2()
    # study_case_test_3()
