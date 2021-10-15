build-coordinator:
	g++ -pthread -std=c++11 -o coordinator.out coordinator/coordinator.cpp utils/*

build-process:
	g++ -pthread -std=c++11 -o process/process.out process/process.cpp utils/*

build: build-process build-coordinator

run-coordinator: build-coordinator
	./coordinator.out

run-process: build-process
	python process/run_study_cases.py