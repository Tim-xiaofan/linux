#include "./init.h"

int main(int argc, char *argv[])
{
    int shmid, semid;
    init(&shmid, &semid);
    //用信号量控制读写
    pid_t pid;
    if ((pid = fork()) == -1)
    {
        printf("fork err\n");
    }
    else if (pid == 0)
    {
        printf("child\n");
        //进程读
        /*while (1)
        {
            sem_p(semid, 0); //申请使用内存
            printf("ouput: %s\n", shm_addr);
            sem_v(semid, 0);
            //if (strcmp(shm_addr, "quit"))
            //break;
        }*/
        if (execv("./read", NULL) < 0)
        {
            printf("execv %s\n", strerror(errno));
            exit(-1);
        }
    }
    else
    {
        //进程写
        /*while (1)
        {
            sem_p(semid, 0); //申请使用内存
            printf("input: ");
            scanf("%s", shm_addr);
            sem_v(semid, 0);
            //if (strcmp(shm_addr, "quit"))
            //break;
        }*/
        printf("parent \n");
        /*sleep(3);
        sem_p(semid, 0); //申请使用内存
        int i;
        item *e;
        e = (item *)malloc(sizeof(item));
        while(!q_is_empty(shm_addr))
        {
            del_q(e, shm_addr);
            printf("%d\n" ,e->data);
        }
        sem_v(semid, 0);*/
        //释放使用内存
        if (execv("./write", NULL) < 0)
        {
            printf("execv %s\n", strerror(errno));
            exit(-1);
        }
    }

    semctl(semid, IPC_RMID, 0);

    shmctl(shmid, IPC_RMID, 0); //销毁

    while (wait(0) != -1);
}