#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "./msfw_q.h"
#include "../11.3信号量通信机制//sem_test.h"

#define SHM_KEY 2056
#define SHM_SIZE 1024

void init(int *shmid, int *semid)
{
    int key = SHM_KEY;
    int size = SHM_SIZE;
    *shmid = shmget(key, size, IPC_CREAT | 0770);
    if (*shmid == -1)
    {
        printf("shmget %s\n", strerror(errno));
        return;
    }
    printf("shmid = %d\n", *shmid);

    //创建二元信号量
    *semid = semget(SEM_KEY, 1, IPC_CREAT | 0700);
    unsigned short a[1] = {1};
    printf("semid = %d\n", *semid);

    union semun args;
    args.array = a;
    int ret = semctl(*semid, 0, SETALL, args);
    if (ret == -1)
    {
        printf("semctl %s\n", strerror(errno));
    }
    //sem_v(semid, 0);
    printf("sem's val = %d\n", semctl(*semid, 0, GETVAL));
}

