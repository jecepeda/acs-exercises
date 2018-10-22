compile:
	gcc ex1/ex1.c -o ex1.out
	gcc -fopenmp ex1/ex1-parallel.c -o ex1-parallel.out
	gcc ex2/ex2-serial.c -o ex2-serial.out
	gcc -fopenmp ex2/ex2-parallel.c -o ex2-parallel.out -lm
	gcc ex3/ex3-serial.c -o ex3-serial.out
	gcc -fopenmp ex3/ex3-parallel.c -o ex3-parallel.out

run-perf:
	find . -type f -name "*.out" -exec sudo perf stat -r 10 {} \;

perf-ex1:
	sudo perf stat -r 100 ./ex1.out
	sudo perf stat -r 100 ./ex1-parallel.out

perf-ex2:
	sudo perf stat -r 100 ./ex2-serial.out
	sudo perf stat -r 100 ./ex2-parallel.out

perf-ex3:
	sudo perf stat -r 10 ./ex3-serial.out
	sudo perf stat -r 10 ./ex3-parallel.out

clean:
	find . -type f -name "*.out" -exec rm {} \;
