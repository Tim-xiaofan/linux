#include "./init.h"

int main(int argc, char *argv[])
{
    int shmid, semid;
    init(&shmid, &semid);

    msfw_q *shm_addr;
    shm_addr = (msfw_q *)shmat(shmid, 0, 0); //挂载内存
    init_q(shm_addr);
    if (shm_addr == (msfw_q *)-1)
    {
        printf("shmat %s", strerror(errno));
        return -1;
    }
    int i = 0;
    item* e = (item*)malloc(sizeof(item));
    while(i <= 26)
    {
        sem_p(semid, 0);
        //print_q(shm_addr);
        if(!is_empty(shm_addr))
        {
            i++;
            out_q(shm_addr, e);
            printf("read%d:%c\n", i, e->data);
        }
        sem_v(semid, 0);
    }
    semctl(semid, IPC_RMID, 0);
    shmctl(shmid, IPC_RMID, 0); //销毁
    if(shmdt(shm_addr) == -1)
        printf("shmdt is failed.\n");
}
