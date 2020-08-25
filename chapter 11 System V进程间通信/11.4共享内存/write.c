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
    int i;
    item* e = (item*)malloc(sizeof(item));
    for(i = 0; i <= 26; i++)
    {
        sem_p(semid, 0);
        e->data = 'a' + i;
        print_q(shm_addr);
        en_q(shm_addr, e);
        sem_v(semid, 0);
    }
    semctl(semid, IPC_RMID, 0);
    shmctl(shmid, IPC_RMID, 0); //销毁
    if(shmdt(shm_addr) == -1)
        printf("shmdt is failed.\n");
}

void q_test()
{
    msfw_q *q = (msfw_q*)malloc(sizeof(msfw_q));
    init_q(q);

    int i;
    item* e = (item*)malloc(sizeof(item));
    for(i = 0; i <26; i++)
    {
        e->data = 'a' + i;
        en_q(q, e);
    }
    printf("items = %d\n", get_items(q));
    print_q(q);

    out_q(q, e);
    printf("head :%c\n", e->data);
    while(!is_empty(q))
    {
        out_q(q, e);
        printf("%c ", e->data);
    }
    printf("\n");
}