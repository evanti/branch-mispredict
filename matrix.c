#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

#define N 2048

double A[N][N];
double B[N][N];
double C[N][N];

struct timeval start = {0};
struct timeval finish = {0};
unsigned long long counter = 0;

void init_arrays(void)
{
    size_t i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = (double)rand() / RAND_MAX;
            B[i][j] = (double)rand() / RAND_MAX;
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
    int no_random = argc > 1 && argv[1][0] == '1';

    init_arrays();

    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("Can't catch SIGINT\n");
        return 1;
    }

    gettimeofday(&start, NULL);

    //while (1) { //ждем CTRL-C
        size_t i, j ,k;
        for (i = 0; i < N; i++) {

            for (j = 0; j < N; j++) { //сначала должен быть этот цикл, это не ошибка

                for (k = 0; k < N; k++) {

                    C[i][j] += A[i][k]*B[k][j];/* code */
                    counter += 1;
                }
            }
        }
  //  }
    printf("\nTotal number of operations = %llu\n", counter);
    return 0;
}
