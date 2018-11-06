#include <stdio.h>

void primeNumbers(int n);

int main(){
    int n = 100000;
    primeNumbers(n);
}


void primeNumbers(int number){
    int j, i = 0;
    int primes[number+1];

    //populating array with naturals numbers
    for(i = 0; i<=number; i++)
        primes[i] = i;
    #pragma omp parallel
    {
        #pragma omp for schedule(guided, 2000)
        for(i = number; i > 0; --i){
            for(j = 2; j < i; j++){
                if (i % j == 0){
                    primes[i] = 0;
                    break;
                }
            }
        }
    }

    int nOfPrimes = 0;
    for(i = 2; i<=number; i++){
        //If number is not 0 then it is prime
        if (primes[i]!=0){
            nOfPrimes++;
            // printf("%d %d\n",primes[i], nOfPrimes);
        }
    }
}
