#include <stdlib.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <pthread.h>

const int N = 10000;
const char *cmd = "";

void routine()
{
    int pid;
    if (pid = fork())
        exit(0);
    else if (pid < 0)
        exit(1);
    sleep(100);
}

int main(int argc, char *argv[])
{
    pthread_t threads[N];
    if (argc < 2) {
        printf("usage: %s num\n", argv[0]);
        return -1;
    }

    int thread_num = atoi(argv[1]);
    printf("thread_num: [%d]\n", thread_num);

    int i;
    for (i = 0; i < thread_num; ++i) {
        if (0 != pthread_create( &threads[i], NULL, (void *)(*routine), NULL)) {
            printf("Thread creation failed. num: %d\n", i);
            break;
        }
        else {
            // printf("%d\n", i);
        }
    }
    return 0;
}

