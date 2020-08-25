#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct node1
{
    void* data;
} item;

typedef struct node2
{
    item i;
    struct node2* next;
}node;

typedef struct m_msfw_q
{
    node* front;
    node* rear;
    int items;
}msfw_q;

void init_q(msfw_q* q){
    q->front = q->rear = NULL;
    q->items = 0;
}

int q_is_empty(msfw_q* q)
{
    if(q->items == 0)return 1;
    else return 0;
}

void copy2node(node* pn, item* i)
{
    pn->i.data = i->data;
}

int enter_q(item* i, msfw_q* q)
{
    node* pn;
    pn = (node*)malloc(sizeof(node));
    if(pn == NULL){
        printf("malloc %s\n", strerror(errno));
        return -1;
    }
    copy2node(pn, i);
    pn->next = NULL;
    if(q_is_empty(q)){//第一个入队元素
        q->front = pn;
        q->rear = pn;
    }
    else
    {
        q->rear->next = pn;
        q->rear = pn;
    }
    q->items++;
    return 0;
}

void copy2item(item* i, node* pn)
{
    i->data = pn->i.data;
}
int del_q(item* i, msfw_q* q)
{
    node* pn;
    if(q_is_empty(q))return -1;
    copy2item(i, q->front);
    pn = q->front;
    q->front = q->front->next;
    free(pn);
    q->items--;
    if(q->items == 0)q->rear = NULL;
    return 0;
}

void empty_q(msfw_q* q){
    item i;
    while (!q_is_empty(q))
    {
        del_q(&i, q);
    }   
}

int get_shm(int shm_size)
{
    int shmid = 0;
    int key = ftok(".", 10);
    shmid = shmget(key, shm_size, IPC_CREAT|0770);
    if(shmid == -1)
    {
        printf("shmget %s\n", strerror(errno));
        return -1;
    }
    return shmid;
}