# How to build and run:
```bash
$ cd third_experiment/coordinator/

$ g++ -pthread -std=c++11 -o coordinator.out coordinator.cpp ../utils/*

$ cd third_experiment/process/

$ g++ -pthread -std=c++11 -o process.out process.cpp ../utils/*

$ ./coordinator

$ python run_study_cases.py
```