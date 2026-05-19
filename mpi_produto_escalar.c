// Programação Paralela em C usando MPI
// Produto escalar.
// Este programa realiza o cálculo do produto escalar usando multiprocessamento.
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    srand(time(NULL));
    int tag = 0;
    int n = atoi(argv[1]);
    double *x;
    double *y;
    // Inicializacao MPI
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    int fracao = n/size;
    int resto = n % size;
    int n_elementos;
    if (rank == 0) {
        x = (double*)malloc(sizeof(double)*n);
        y = (double*)malloc(sizeof(double)*n);
        for (int i = 0; i < n; i++) {
            x[i] = rand() % 100;
            y[i] = rand() % 100;
            printf("PROCESSO 0: Vetor %d = %f, %f\n",i, x[i],y[i]);
        }
        int pos;
        for (int i = 1; i < size; i++) {
            pos = fracao * i;
            printf("Pos = %d\n", pos);
            double *p_dados;
            p_dados = &x[pos];
            if (i == size-1)
                n_elementos = fracao + resto;
            else
                n_elementos = fracao;
            MPI_Send(p_dados, n_elementos, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            p_dados = &y[pos];
            MPI_Send(p_dados, n_elementos, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        }
        // acabando de enviar os dados, computo a parte do processo 0
        double soma = 0;
        for (int i = 0; i < n_elementos; i++) {
            soma = soma + (x[i] * y[i]);
        }
        // aguardo a resposta de cada processo, para receber a soma parcial e realizar a soma final
        double p_soma;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&p_soma, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
            soma = soma + p_soma;
        }
        // imprimo o produto escalar total
        printf("SOMA TOTAL = %f\n", soma);
    }
    else {
        if (rank == size-1)
            n_elementos = fracao + resto;
        else
            n_elementos = fracao;
        x = (double*)malloc(sizeof(double)*n_elementos);
        y = (double*)malloc(sizeof(double)*n_elementos);
        printf("Rank = %d\nNumero de Elementos = %d\n", rank, n_elementos);
        MPI_Recv(x, n_elementos, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(y, n_elementos, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        double soma = 0;
        for (int i = 0; i < n_elementos; i++) {
            printf("Vetor na posicao %d = %f,%f\n", i, x[i],y[i]);
            soma = soma + (x[i]*y[i]);
        }
        MPI_Send(&soma, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
