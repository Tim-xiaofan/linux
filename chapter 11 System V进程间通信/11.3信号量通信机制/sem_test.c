#include "./sem_test.h"

int main(int argc, char* argv[])
{
    unsigned short a[2] = {4, 6};
    int sem = get_sem(2);

    //初始化
    union semun args;
    args.array = a;
    semctl(sem, 0, SETALL, args);

    unsigned short b[2]; //取出所有信号量
    args.array = b;
    semctl(sem, 0 , GETALL, args);
    int i;
    for(i = 0; i <2; i++){
        printf("sem %d's val = %d\n", b[i]);
    }

    //依次取出
    printf("\n");
    sem_p(sem, 0);
    sem_v(sem, 1);
    for(i = 0; i <2; i++){
        printf("sem %d's val = %d\n", i, semctl(sem, i, GETVAL));
    }

    //信号量的操作
    printf("sem id %d\n", sem);
    semctl(sem, 0, IPC_RMID, 0);//删除信号量集合
}