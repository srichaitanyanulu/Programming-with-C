#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
int main() {
  int shmid;
  key_t key;
  key=ftok("sample1.txt","a");
  shmid=shmget(key,512,0666);
  printf("id=%d\n",shmid);
  char *str=(char*)shmat(shmid,NULL,0);
  printf("read the data from the segment = %s\n",str);
  shmdt(str);
  return 0;
}
