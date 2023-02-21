#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

void my_handler(int signo){
  printf("broken pipe signal = %d \n",signo);
}

int main()
{
  signal(SIGPIPE,my_handler);
  int fds[2];
  int pid;
  char buf[10];
  pipe(fds);
  pid=fork();
  if(pid==0){
	close(fds[0]);
	close(fds[1]);
  }
  else{
	sleep(2);
	close(fds[0]);
	write(fds[1],"Hello",6);
  }
  return 0;
}
