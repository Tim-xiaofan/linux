#include "shm_test.h"
#include "sem_test.h"

int main(int argc, char *argv[])
{
    int shmid = 0;
    shmid = get_shm(1024);
    if (shmid == -1)
    {
        printf("shmget %s\n", strerror(errno));
        return -1;
    }
    printf("shmid = %d\n", shmid);
    shmctl(shmid, IPC_RMID, 0);

    msfw_q *q;
    init_q(q);
    
    int i;
    item *e;
    e = (item *)malloc(sizeof(item));
    for (i = 0; i < 10; i++)
    {
        e->data = &i;
        enter_q(e, q);
    }
    printf("before items = %d\n", q->items);
    //empty_q(q);
    printf("after empty items = %d\n", q->items);
    for (i = 0; i < 10; i++)
    {
        del_q(e, q);
        printf("%d\n", *(int *)e->data);
    }
    printf("after items = %d\n", q->items);
}