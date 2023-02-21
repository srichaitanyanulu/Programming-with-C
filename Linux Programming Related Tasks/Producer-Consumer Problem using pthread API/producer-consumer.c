#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

int buf[50];
int num=0;
int i=0;
int front=0,rear=0;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv1=PTHREAD_COND_INITIALIZER;
pthread_cond_t cv2=PTHREAD_COND_INITIALIZER;

void insert()
{
    printf("produced: %d\n",i);
    buf[num]=i++;
    rear=(rear+1)%2;
    num=num+1;
}
void delete()
{
    int d=buf[front];
    printf("consumed: %d\n",d);
    front=(front+1)%2;
    num-=1;
return ;
}
void * producer(void *arg)
{
    int j;
    for(j=0;j<5;j++)
    {
    pthread_mutex_lock(&m);
    while(num==2)
    pthread_cond_wait(&cv1,&m);
    insert(i);
    pthread_cond_signal(&cv2);
    pthread_mutex_unlock(&m);
    }
}
void * consumer(void *arg)
{
    int i;
    for(i=0;i<5;i++)
    {
    pthread_mutex_lock(&m);
    while(num==0)
    pthread_cond_wait(&cv2,&m);
    delete();
    pthread_cond_signal(&cv1);
    pthread_mutex_unlock(&m);
    }
}
int main(){
    pthread_t ct,pt;
    void *ptr;
    void *ret;
    pthread_create(&pt,NULL,producer,ptr);
    pthread_create(&ct,NULL,consumer,ptr);
    pthread_join(pt,&ret);
    pthread_join(ct,&ret);
return 0;
}
