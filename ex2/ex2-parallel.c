#include <stdio.h>
#include <omp.h>

void primeNumbers(int n);

int main(){
    int n = 10000;
    primeNumbers(n);
}


void primeNumbers(int number){
    int j, i = 0;
    int primes[number+1];

    #pragma omp parallel
    {
        #pragma omp for schedule (static)
        for(i = 2; i<=number; i++)
            primes[i] = i;
        i = 2;
        #pragma omp for schedule (guided, 50)
        for(i=2; i*i <=number; i++){
            if (primes[i] != 0){
                for(j=2; j*i<number+1; j++){
                    primes[primes[i]*j]=0;
                }
            }
        }
    } // end parallelism
    for(i = 2; i<=number; i++)
    {
        //If number is not 0 then it is prime
        if (primes[i]!=0){
            // printf("%d\n",primes[i]);
        }
    }
}
