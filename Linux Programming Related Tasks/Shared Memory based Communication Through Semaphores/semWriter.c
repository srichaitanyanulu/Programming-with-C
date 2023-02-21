#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<unistd.h>

#define READDATA 0
#define WROTEDATA 1

int  main(void)
{
	key_t  key;
	int  shmid, semid, i ;
	int *pShm = NULL;
	
	struct sembuf sb;

	key = ftok("SemShmWriter.c" , 'M' );

	/*request the kernel to give block of shm */

	if (   (shmid = shmget ( key , sizeof(int) , IPC_CREAT | 0777 )) < 0 )
	{
		perror("shmget");
		exit(1);
	}
	
	/* request the kernel to allot 2 sems */

	if( (semid = semget(key , 2, IPC_CREAT | 0777 )) < 0 )
	{
		perror("semget");
		exit(1);
	}

	/* attach the shm object to user addrress space */

	if ( (pShm = (int*)shmat ( shmid , NULL , 0)) == NULL )
	{
		perror("shmat");
		exit(1);
	}

	/* initialize READDATA to 1 */

	semctl ( semid , READDATA , SETVAL , 1 );

	/* write to the shm array*/
	
	for ( i = 0 ; i < 15 ; i ++ )
	{

		/*wait until the previous item is read */

		sb.sem_num = READDATA;
		sb.sem_op =  -1 ; /*wait */
		sb.sem_flg = SEM_UNDO;
		semop ( semid , &sb , 1 );

		printf("Wrote %d\n" , *(pShm) = (i+1)*10 ); 		/*Write to shm object */

		/*inform the reader that new item is written*/

		sb.sem_num = WROTEDATA;
		sb.sem_op =  1 ; /*release  */
		sb.sem_flg = SEM_UNDO;
		semop ( semid , &sb , 1 );
		sleep(2);
	}

	printf("Wrote the data to shm \n");

	/*detach shm */

	getchar();
	shmdt ( pShm );

	/* remove the IPC object shm */

	shmctl( shmid , IPC_RMID , NULL );
	return 0;

} 


