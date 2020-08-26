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
    pid_t pid = getpid();
    int i, num;
    item *e = (item *)malloc(sizeof(item));
    for (i = 0; i < 26; i++)
    {
        ///申请一个产品
        sem_p(semid, FULL);
        //申请缓冲区使用权
        sem_p(semid, MUTEX);
        //sleep 0~3s
        num = get_rand_num();
        sleep(num);
        //取出字母
        out_q(shm_addr, e); //消费一个产品
        printf("消费者%d消费%c		缓冲区:", pid, e->data);
        print_q(shm_addr);
        printf("\n");
        //归还缓冲区使用权
        sem_v(semid, MUTEX);
        //释放一个空的缓冲区
        sem_v(semid, EMPTY);
    }
    semctl(semid, IPC_RMID, 0);
    shmctl(shmid, IPC_RMID, 0); //销毁
    if (shmdt(shm_addr) == -1)
        printf("shmdt is failed.\n");
}
