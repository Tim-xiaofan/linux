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
    item* e = (item*)malloc(sizeof(item));
    for(i = 0; i < 26; i++)
    {
        //申请一个空缓冲区
		sem_p(semid, EMPTY);
		//申请缓冲区使用权
		sem_p(semid, MUTEX);
        //sleep 0~3s
        num = get_rand_num();
        sleep(num);
		//放入一个字母
        char ch = 'a' + i;
        e->data = ch;
        en_q(shm_addr, e);
		printf("生产者%d生产%c		缓冲区:", pid, ch);
		print_q(shm_addr);
		printf("\n");
		//归还缓冲区使用权
		sem_v(semid, MUTEX);
		//释放一个产品
		sem_v(semid, FULL);
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