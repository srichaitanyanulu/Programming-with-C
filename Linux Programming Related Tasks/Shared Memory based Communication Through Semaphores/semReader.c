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

	if (   (shmid = shmget ( key , sizeof(int) ,  0777 )) < 0 )
	{
		perror("shmget");
		exit(1);
	}

	if( (semid = semget(key , 2,  0777 )) < 0 )
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

	/* write to the shm array*/

	for ( i = 0 ; i < 15 ; i ++ )
	{

		/*wait until the new item is written */

		sb.sem_num = WROTEDATA;
		sb.sem_op =  -1 ; /*wait */
		sb.sem_flg = SEM_UNDO;
		semop ( semid , &sb , 1 );

		printf("Reader read: %d\n" , *(pShm) ); 	/*Write to shm object */

		/*inform the writer that item is read */

		sb.sem_num = READDATA;
		sb.sem_op =  1 ; /*release  */
		sb.sem_flg = SEM_UNDO;
		semop ( semid , &sb , 1 );

	}

	printf("Read all the data from shm \n");

	/*detach shm */

	getchar();
	shmdt ( pShm );
	
	/* remove the IPC object shm */

	shmctl( shmid , IPC_RMID , NULL );
	semctl(semid , READDATA , IPC_RMID, NULL );
	semctl(semid , WROTEDATA , IPC_RMID , NULL );
	return 0;

} 


