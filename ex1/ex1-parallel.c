#include <stdio.h>
#include <omp.h>
long num_steps = 1000000;
double step = 1.0/1000000.0;
int main(){
    double pi, sum = 0.0;
    #pragma omp parallel
    {
        double x = 0.0;
        int i;
        # pragma omp for reduction (+:sum) schedule (guided, 1000)
        for(i = 0; i < num_steps; ++i){
            x = (i - 0.5) * step;
            sum += 4.0 / (1.0 + x*x);
        }
    } // Threads terminate and sum is joined

    pi = step*sum;
    printf("PI VALUE = %f\n", pi);
}
