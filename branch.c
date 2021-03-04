#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <immintrin.h>

#ifndef N
#define N 2048
#endif
#define MAX_STATIC 4096

#define MID_RANGE 0x8000000000000000ULL

#ifdef NORAND
#define CHECK_RANDOM1 \
    rand_val = rv
#define CHECK_RANDOM2 \
    rand_val = rv
#define CHECK_RANDOM3 \
    rand_val = rv
#else
    #ifndef NORAND1
    #define CHECK_RANDOM1 \
        if (rv < MID_RANGE) continue
    #else
    #define CHECK_RANDOM1 \
        rand_val = rv
    #endif
    #ifndef NORAND2
    #define CHECK_RANDOM2 \
        if (rv < MID_RANGE) continue
    #else
    #define CHECK_RANDOM2 \
        rand_val = rv
    #endif
    #ifndef NORAND3
    #define CHECK_RANDOM3 \
        if (rv < MID_RANGE) continue
    #else
    #define CHECK_RANDOM3 \
        rand_val = rv
    #endif
#endif

volatile unsigned long long rand_val = 0ULL;

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
volatile unsigned long long counter = 0ULL;

unsigned long long result = 0ULL;

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
            unsigned long long rv = 0ULL;
            _rdrand64_step(&rv);
            A[i][j] = (double)rv / MID_RANGE / 2;
            _rdrand64_step(&rv);
            B[i][j] = (double)rv / MID_RANGE / 2;
            C[i][j] = 0.0;
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
        printf("\nTotal number of operations = %llu\n", counter);
        printf("Total time of operations = %f s\n", elapsed);
        printf("Operations per second = %llu\n", (unsigned long long)(counter / elapsed));
        exit(0);
    }
}

int main(int argc, char **argv)
{
    unsigned long long rv = 0;
    
    init_arrays();

    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("Can't catch SIGINT\n");
        return 1;
    }
    
    gettimeofday(&start, NULL);
    
    while (1) { //ждем CTRL-C
        size_t i, j ,k;
        for (i = 0; i < N; i++) {
            _rdrand64_step(&rv);
            CHECK_RANDOM1;
            for (k = 0; k < N; k++) { //сначала должен быть этот цикл, это не ошибка
                _rdrand64_step(&rv);
                CHECK_RANDOM2;
                for (j = 0; j < N; j++) {
                    _rdrand64_step(&rv);
                    CHECK_RANDOM3;
                    C[i][j] += A[i][k]*B[k][j];/* code */
                    counter += 1;
                }
            }
        }
    }

    return 0;
}
