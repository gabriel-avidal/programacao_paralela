// Calcula o valor de pi usando integral.
// Utiliza programação paralela, subdividindo n passos pelas threads.
// O valor de n é passado como parâmetro na linha de comando.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int n, i;
    double mypi, h, sum, x;

    n = atoi(argv[1]);
    h = 1.0 / (double) n;
    sum = 0.0;
    int num_threads;
    int id;

    #pragma omp parallel private(x,id) shared(h,n,num_threads) reduction(+:sum)
    id = omp_get_thread_num();
    num_threads = omp_get_num_threads();
    for (i=id*(n/num_threads);i<(id+1)*(n/num_threads);i++) {
        x = h*((double)i-0.5);
        sum += 4.0 / (1.0 + x*x);
    }
    mypi = h*sum;

    printf("pi is approximately %.16f\n", mypi);
    return 0;
}
