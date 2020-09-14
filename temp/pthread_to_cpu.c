#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

void *testfunc(void *arg)
{
    int i, cpus = 0;
    cpu_set_t mask;
    cpu_set_t get;

    cpus = sysconf(_SC_NPROCESSORS_CONF);
    printf("this system has %d processor(s)\n", cpus);

    CPU_ZERO(&mask);
    for (i = 0; i < 4; i++) { /*将0、1、2、3添加到集合中*/
        CPU_SET(i, &mask);
    }

    /* 设置cpu 亲和性(affinity)*/
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0) {
        fprintf(stderr, "set thread affinity failed\n");
    }

    /* 查看cpu 亲和性(affinity)*/
    CPU_ZERO(&get);
    if (pthread_getaffinity_np(pthread_self(), sizeof(get), &get) < 0) {
        fprintf(stderr, "get thread affinity failed\n");
    }

    /* 查看当前线程所运行的所有cpu*/
    for (i = 0; i < cpus; i++) {
        if (CPU_ISSET(i, &get)) {
            printf("this thread %d is running in processor %d\n", (int)pthread_self(), i);
        }
    }
    sleep(3); //查看

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, (void *)testfunc, NULL) != 0) {
        fprintf(stderr, "thread create failed\n");
        return -1;
    }

    pthread_join(tid, NULL);
    return 0;
}