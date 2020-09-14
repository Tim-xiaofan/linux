#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void helloworld(char *agrv);
void helloworld(char *argv)
{
    int *p = (int *)malloc(sizeof(int) * 10);      //显示的申请堆空间
    printf("the child id = %lu\n", pthread_self()); //打印线程ID
    printf("the message is %s\n", argv);           //打印参数
    memset(p, 'c', 10);
    printf("p = %x\n", p);
    pthread_exit(p); //设置退出值
}

int main(int argc, char *argv[])
{
    pthread_t t_id, t_id1;
    int *retval;
    void (*p_func)(char *);
    p_func = helloworld;
    /* 创建线程*/
    pthread_create(&t_id, NULL, (void *)p_func, "helloworld");
    pthread_create(&t_id1, NULL, (void *)p_func, "helloworld");
    /* 等待子线程结束*/
    pthread_join(t_id, (void **)&retval);
    pthread_join(t_id1, (void **)&retval);
    printf("retval = %x, *retval = %c\n", retval, *retval);
    *retval = 'd';
    printf("after modified retval = %x, *retval = %c\n", retval, *retval);
    return 0;
}
