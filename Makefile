compile:
	gcc ex1/ex1.c -o ex1.out
	gcc ex1/ex1-parallel.c -o ex1-parallel.out
	gcc ex2/ex2-serial.c -o ex2-serial.out
	gcc ex2/ex2-parallel.c -o ex2-parallel.out

run-perf:
	find . -type f -name "*.out" -exec sudo perf stat -r 10 {} \;

clean:
	find . -type f -name "*.out" -exec rm {} \;
