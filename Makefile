compile:
	gcc ex1/ex1.c -o ex1.out
	gcc -fopenmp ex1/ex1-parallel.c -o ex1-parallel.out
	gcc ex2/ex2-serial.c -o ex2-serial.out
	gcc -fopenmp ex2/ex2-parallel.c -o ex2-parallel.out -lm
	gcc ex3/ex3-serial.c -o ex3-serial.out
	gcc -fopenmp ex3/ex3-parallel.c -o ex3-parallel.out
	# mpi
	mpicc -o ex1-mpi.out ex1/ex1-mpi.c
	mpicc -o ex2-mpi.out ex2/ex2-mpi.c
	mpicc -o ex3-mpi.out ex3/ex3-mpi.c


run-perf:
	find . -type f -name "*.out" -exec sudo perf stat -r 10 {} \;

perf-ex1:
	sudo perf stat -r 10 ./ex1.out
	sudo perf stat -r 10 ./ex1-parallel.out
	sudo perf stat -r 10 /home/jcepeda/mpich-install-3.2/bin/mpirun -np 4 -hosts jcepeda ./ex1-mpi.out

perf-ex2:
	sudo perf stat -r 10 ./ex2-serial.out
	sudo perf stat -r 10 ./ex2-parallel.out
	sudo perf stat -r 10 /home/jcepeda/mpich-install-3.2/bin/mpirun -np 4 -hosts jcepeda ./ex2-mpi.out

perf-ex3:
	sudo perf stat -r 10 ./ex3-serial.out
	sudo perf stat -r 10 ./ex3-parallel.out
	sudo perf stat -r 10 /home/jcepeda/mpich-install-3.2/bin/mpirun -np 4 -hosts jcepeda ./ex3-mpi.out

clean:
	find . -type f -name "*.out" -exec rm {} \;
