#include <stdio.h>

void primeNumbers(int n);

int main(){
    int n = 10000;
    primeNumbers(n);
}


void primeNumbers(int number){
    int j, i = 0;
    int primes[number+1];

    //populating array with naturals numbers
    for(i = 2; i<=number; i++)
        primes[i] = i;

    i = 2;
    for(i=2; i*i <=number; i++){
        if (primes[i] != 0){
            for(j=2; j*i<number+1; j++){
                    primes[primes[i]*j]=0;
            }
        }
    }

    int nOfPrimes = 0;
    for(i = 2; i<=number; i++)
    {
        //If number is not 0 then it is prime
        if (primes[i]!=0){
            nOfPrimes++;
            // printf("%d %d\n",primes[i], nOfPrimes);
        }
    }
}
