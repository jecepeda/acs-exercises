c-ex1:
	gcc ex1/ex1.c -o ex1.out
	gcc ex1/ex1-parallel.c -o ex1-parallel.out

run-perf:
	find . -type f -name "*.out" -exec sudo perf stat -r 10 {} \;
