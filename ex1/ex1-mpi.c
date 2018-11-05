#include <mpi.h>
#include <stdio.h>

long num_steps = 1000000;
double step = 1.0/1000000.0;

int main(int argc, char **argv){
    int i, process_rank;
    int lower_limit, upper_limit;
    double x, pi, sum = 0.0;
    double local_sum = 0.0;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    lower_limit = process_rank * (num_steps / 4);
    upper_limit = lower_limit + (num_steps / 4);

    for(i = lower_limit; i < upper_limit; i++){
        x = (i - 0.5) * step;
        local_sum += 4.0 / (1.0 + x*x);
    }
    MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (process_rank == 0){
        pi = step*sum;
        printf("PI VALUE = %f\n", pi);
    }
    MPI_Finalize();
}
