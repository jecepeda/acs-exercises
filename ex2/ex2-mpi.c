#include <mpi.h>
#include <stdio.h>

void primeNumbers(int n);

int main(){
    int n = 100000;
    primeNumbers(n);
}


void primeNumbers(int number){
    int j, i = 0;
    int primes[number];
    int process_rank, n_of_processes;
    int lower_limit, upper_limit;
    int n_of_primes = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &n_of_processes);
    int partial_primes[number / n_of_processes];
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    lower_limit = process_rank * (number / n_of_processes);
    upper_limit = lower_limit + (number / n_of_processes);

    for (i=0; i < number / n_of_processes; i++){
        partial_primes[i] = i + lower_limit;
    }
    for(i = 0; i < number / n_of_processes; i++){
        for(j = 2; j < lower_limit + i; j++){
            if ((lower_limit + i) % j == 0){
                partial_primes[i] = 0;
                break;
            }
        }
    }
    MPI_Gather(partial_primes, number / n_of_processes, MPI_INT, primes, number / n_of_processes, MPI_INT, 0, MPI_COMM_WORLD);
    if (process_rank == 0){
        for(i = 2; i<=number; i++){
            //If number is not 0 then it is prime
            if (primes[i]!=0){
                n_of_primes++;
                // printf("%d %d\n",primes[i], n_of_primes);
            }
        }
    }
    MPI_Finalize();
}
