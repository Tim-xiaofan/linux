#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

//const int sems_count 2;

/*信号量的p操作*/
int sem_p (int semid, int semnum){
    struct sembuf op;
    op.sem_num = semnum;
    op.sem_op = -1;
    op.sem_flg = 0;//默认操作
    if(semop(semid, &op, 1) == -1){
        printf("semop %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/*信号量的v操作*/
int sem_v (int semid, int semnum){
    struct sembuf op;
    op.sem_num = semnum;
    op.sem_op = 1;
    op.sem_flg = 0;//默认操作
    if(semop(semid, &op, 1) == -1){
        printf("semop %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int get_sem(const int sems_count)
{

    int sem;
    key_t key;
    if ((key = ftok(".", 'A')) == -1)
    { //获取key值
        printf("ftok:%s\n", strerror(errno));
        return -1;
    }
    if ((sem = semget(key, sems_count, IPC_CREAT | 0770)) == -1)
    { //创建信号量集合，包含2个信号量
        printf("semget:%s\n", strerror(errno));
        return -1;
    }
    return sem;
}