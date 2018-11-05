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
    int process_rank;
    int partial_primes[number / 4];
    int lower_limit, upper_limit;
    int n_of_primes = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    lower_limit = process_rank * (number / 4);
    upper_limit = lower_limit + (number / 4);

    for (i=0; i < number / 4; i++){
        partial_primes[i] = i + lower_limit;
    }
    for(i = 0; i < number / 4; i++){
        for(j = 2; j < lower_limit + i; j++){
            if ((lower_limit + i) % j == 0){
                partial_primes[i] = 0;
                break;
            }
        }
    }
    MPI_Gather(partial_primes, number / 4, MPI_INT, primes, number / 4, MPI_INT, 0, MPI_COMM_WORLD);
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
