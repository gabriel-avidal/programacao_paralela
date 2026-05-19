// Calcula o valor de pi usando integral
// Utiliza programação paralela com o MPI (múltiplos processos).
// Gabriel Vidal, 2026
// gabriel@uerj.br
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int n, i, j;
    double mypi, h, sum, x;

    int p, my_rank;
    int n_bar;
    double sum_local, mypi_local;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Envio do valor de n para todos os processo
    if (my_rank == 0) {
        n = atoi(argv[1]);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Processo = %d / Valor de n = %d\n", my_rank, n);
    n_bar = n/p;
    h = 1.0 / (double) n;
    sum = 0.0;
    sum_local = 0.0;
    for (i=0;i<n_bar;i++) {
        j = i + (my_rank*n_bar);   // j eh igual ao i somando o seu displacement (deslocamento).
        x = h*((double)j-0.5);
        sum_local += 4.0 / (1.0 + x*x);
    }
    MPI_Reduce(&sum_local, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    mypi = h*sum;

    if (my_rank == 0)
        printf("pi is approximately %.16f\n", mypi);
    MPI_Finalize();
    return 0;
}
