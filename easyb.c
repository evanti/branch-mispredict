#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

#ifndef N
#define N 2048
#endif
#ifndef Q
#define Q 0.5
#endif
#define MAX_STATIC 4096

#if N > MAX_STATIC
double** A;
double** B;
double** C;
#else
double A[N][N];
double B[N][N];
double C[N][N];
#endif

struct timeval start = {0};
struct timeval finish = {0};
volatile unsigned long long counter = 0;
volatile unsigned long long counter2 = 0;

void init_arrays(void)
{
    size_t i, j;

#if N > MAX_STATIC
    A = (double**)calloc(N, sizeof(double*));
    B = (double**)calloc(N, sizeof(double*));
    C = (double**)calloc(N, sizeof(double*));
    if (!A || !B || !C) {
        fprintf(stderr, "Memory allocation error!\n");
        exit(1);
    }
#endif

    for (i = 0; i < N; i++) {
#if N > MAX_STATIC
        A[i] = (double*)calloc(N, sizeof(double));
        B[i] = (double*)calloc(N, sizeof(double));
        C[i] = (double*)calloc(N, sizeof(double));
        if (!A[i] || !B[i] || !C[i]) {
            fprintf(stderr, "Memory allocation error!\n");
            exit(1);
        }
#endif

        for (j = 0; j < N; j++) {
            A[i][j] = (double)rand() / RAND_MAX;
            B[i][j] = (double)rand() / RAND_MAX;
            C[i][j] = 0.0;
            // printf("%d\n", RAND_MAX );
        }
    }
}

void sig_handler(int signo)
{
    // при перехвате:
    if (signo == SIGINT) {
        double elapsed;

        gettimeofday(&finish, NULL);
        elapsed = finish.tv_sec - start.tv_sec
                + (finish.tv_usec - start.tv_usec) / 1000000.0;
        printf("Current Q = %f\n", Q);
        printf("Total number of attempts = %llu\n", counter2);
        printf("Total number of operations = %llu\n", counter);
        printf("Total time of operations = %f s\n", elapsed);
        printf("Operations per second = %llu\n", (unsigned long long)(counter / elapsed/1000000));
        exit(0);
    }
}

int main(int argc, char **argv)
{
    init_arrays();

    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("Can't catch SIGINT\n");
        return 1;
    }

    gettimeofday(&start, NULL);

    while (1) { //ждем CTRL-C
        size_t i, j ,k;
        for (i = 0; i < N; i++) {
            for (k = 0; k < N; k++) {
                for (j = 0; j < N; j++) {
                    counter2+=1;
                    if (B[k][j]>Q) {
                      continue;
                    }
                    C[i][j] += A[i][k]*B[k][j];
                    counter += 1;
                }
            }
        }
    }

    return 0;
}
