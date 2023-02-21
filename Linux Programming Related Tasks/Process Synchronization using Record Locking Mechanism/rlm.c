#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int args,char* argv[])
{
    struct flock fl = { F_UNLCK,SEEK_SET,0,100,0};
    int fsize,offset,fd,pid;
    char buf[50];

    fd = open(argv[1],O_RDWR);

    fl.l_type = F_WRLCK;
    fl.l_pid = getpid();
    if(fcntl(fd,F_SETLK,&fl) == -1)
    {
   	 printf("Cannot set exclusive lock\n");
   	 exit(1);
    }
    else if(fl.l_type != F_UNLCK && fl.l_type != F_RDLCK)
   	 printf("file has been exclusively locked by process: %d\n",fl.l_pid);
    else{
	printf("File is not locked by any process\n");
	printf("Press <Enter> to lock the file\n");
	printf("********************************");
  }

    printf("Press <Enter> to Release the lock\n");

    getchar();

    fl.l_type =  F_UNLCK;
    printf("File has been Unlocked \n");
    fsize = lseek(fd,0,SEEK_END);
    offset = fsize - 15;

    lseek(fd,offset,SEEK_SET);
  read(fd,buf,15);
    printf("Last  15 bytes of data in the file is: \n");
    printf("********************************************\n");
    printf("%s \n",buf);
    return 0;
}
