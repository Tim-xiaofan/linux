#include "./init.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int shmid, semid;
    init(&shmid, &semid); //创建3个信号量
    //创建1个生产者
    int pid, i;
    for (i = 0; i < 1; i++)
    {
        pid = fork();
        if (pid == -1)
            printf("fork error\n");
        if (pid == 0)
        {
            //printf("test\n");
            if (execv("write", NULL) < 0)
            {
                printf("execv %s\n", strerror(errno));
                exit(-1);
            }
        }
    }
    //创建1个消费者
    if (pid > 0)
    {
        for (i = 0; i < 1; i++)
        {
            pid = fork();
            if (pid == -1)
                printf("fork error\n");
            if (pid == 0)
            {
                //printf("test\n");
                if (execv("read", NULL) < 0)
                {
                    printf("execv %s\n", strerror(errno));
                    exit(-1);
                }
            }
        }
    }
    //主进程最后退出
    printf("before wait\n");
    while (wait(0) != -1);
    printf("after wait\n");
    semctl(semid, IPC_RMID, 0);
    shmctl(shmid, IPC_RMID, 0); //销毁
}