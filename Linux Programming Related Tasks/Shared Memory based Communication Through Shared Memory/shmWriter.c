#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
int main(){
  int shmid;
  key_t key;
  key=ftok("sample1.txt","a");
  shmid=shmget(key,512,IPC_CREAT|0666);
  printf("ID=%d\n",shmid);
  char *str=(char*)shmat(shmid,NULL,0);
  printf("enter the data to be send to the shared memory segment:\n");
  scanf("%s",str);
  printf("data written into the segment\n");
  shmdt(str);
  return 0;
}
