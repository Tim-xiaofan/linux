/*用于共享内存的数据结构*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define BUF_SIZE 512

typedef struct node_item
{
    char data;
} item;

void itemcpy(item *di, const item *si)
{
    di->data = si->data;
}

/*环形缓冲区,循环队列*/
typedef struct node
{
    item data[BUF_SIZE];
    int front, rear;
    bool is_full;
    bool is_empty;
} msfw_q;

/*初始化队列*/
void init_q(msfw_q *q)
{
    q->front = q->rear = 0;
    q->is_full = false;
    q->is_empty = true;
    int i;
}

/*返回队列元素个数*/
int get_items(const msfw_q *q)
{
    return (q->rear - q->front + BUF_SIZE) % BUF_SIZE;
}

/*队尾插入元素*/
bool en_q(msfw_q *q, item *e)
{
    //printf("test\n");
    if ((q->rear + 1) % BUF_SIZE == q->front)
        return false;
    //q->data[q->rear] = e;
    itemcpy(&(q->data[q->rear]), e);
    q->rear = (q->rear + 1) % BUF_SIZE;
    return true;
}

/*队头元素出列*/
bool out_q(msfw_q *q, item *e)
{
    if (q->front == q->rear)
        return false;
    itemcpy(e, &q->data[q->front]);
    q->front = (q->front + 1) % BUF_SIZE;
    return true;
}

/*输出队列*/
void print_q(const msfw_q *q)
{
    printf("q_list:");
    if (q->front == q->rear)
    {
        printf("-");
        return;
    }
    int p = q->front;
    int count = get_items(q);
    int i;
    for (i = 0; i < count; i++)
    {
        printf("%c ", q->data[p].data);
        p = (p + 1) % BUF_SIZE;
    }
    printf("\n");
    //printf("%c ", q->data[front]);
}

/*断队列是否已满*/
bool is_full(msfw_q *q)
{
    if ((q->rear + 1) % BUF_SIZE == q->front)
        return true;
    ;
    return false;
}

bool is_empty(msfw_q *q)
{
    if (q->front == q->rear)
        return true;
    return false;
}

int get_rand_num()
{
    srand((unsigned)(getpid() + time(NULL)));
    return rand() % 4; //0~3s
}