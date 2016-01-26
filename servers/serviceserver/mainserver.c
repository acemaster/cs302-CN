#include "vivek.h"
#include <poll.h>

struct logfds{
	int number;
	int fd[10];
};

//Log Server
int main(int argc,char *argv[]){
	printf("\nMain Server is starting................");
	int fd;
	struct logfds logs;
	struct logfds logwrite;
	int i;
	logs.number=atoi(argv[1]);
	logwrite.number=logs.number;
	for(i=0;i<logs.number;i++){
		char buf[50];
		char buf2[50];
		snprintf(buf, sizeof(buf), "%s%d", "mainfifo", i);
		if(mkfifo(buf,0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		// snprintf(buf2, sizeof(buf2), "%s%d", "mainfifowrite", i);
		// if(mkfifo(buf2,0777) !=0)
		// {
		// 	perror ("The following error occurred");
		// 	exit(0);
		// }
		if((logs.fd[i]=open(buf,O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		// if((logwrite.fd[i]=open(buf2,O_RDWR)) < 0)
		// {
		// 	perror ("The following error occurred");
		// 	exit(0);
		// }
	}

	if(mkfifo("clientfifo",0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
	}
	if(mkfifo("clientfifowrite",0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
	}
	//Shared memory creation and putting log fd info
	int shmid1;
	key_t key1;
	key1=300;
	int size=10000;
	struct logfds *shm;
	if ((shmid1 = shmget (key1, size, IPC_CREAT | 0666)) == -1) {
	   perror("shmget: shmget failed"); exit(1); } else {
	   (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid1);
	}
	shm = (struct logfds *)shmat(shmid1, NULL, 0);
	shm->number=logs.number;
	for(i=0;i<logs.number;i++)
		shm->fd[i]=logs.fd[i];
	//Creating polling fds
	struct pollfd fds[10];
	for(i=0;i<logs.number;i++){
		fds[i].fd=logs.fd[i];
		fds[i].events=POLLRDNORM;
	}
	int temp;
	int j=0;
	int k;
	printf("\nMain fds are created................");
	printf("\nServer started..................");
	while(1){
		int retstatus=poll(fds,logs.number,2000);
		// printf("Polling\n");
		if(retstatus > 0){
			// printf("Got message\n");
			for(j=0;j<logs.number;j++)
			{
				if(fds[j].revents == fds[j].events){
					char buf[100];
					printf("Logged: ");
					if(read(fds[j].fd,buf,100) > 0)
					{
						
					}
					fflush(stdout);
				}
			}
		}
		fflush(stdout); 
	}
	return 0;
}