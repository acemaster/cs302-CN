#include "vivek.h"
#include <poll.h>

struct logfds{
	int number;
	int fd[10];
};

int n_s1;
int n_s2;
//Log Server
int main(int argc,char *argv[]){
	printf("\nMain Server is starting................");
	int fd;
	struct logfds logs;
	struct logfds logwrite;
	int i;
	logs.number=atoi(argv[1]);
	logwrite.number=logs.number;
	n_s1=atoi(argv[2]);
	n_s2=atoi(argv[3]);
	printf("Max clients: %d, S1: %d, S2: %d\n",logs.number,n_s1,n_s2);
	for(i=0;i<logs.number;i++){
		char buf[50];
		char buf2[50];
		snprintf(buf, sizeof(buf), "%s%d", "mainfifo", i);
		if(mkfifo(buf,0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		snprintf(buf2, sizeof(buf2), "%s%d", "mainfifowrite", i);
		if(mkfifo(buf2,0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		if((logs.fd[i]=open(buf,O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		if((logwrite.fd[i]=open(buf2,O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
	}

	if(mkfifo("S1fifo",0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
	}
	if(mkfifo("S2fifo",0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
	}
	if(mkfifo("S1fifowrite",0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
	}
	if(mkfifo("S2fifowrite",0777) !=0)
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
	printf("\nlog fds are created................");
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
						printf("%s\n",buf);
						if(strcasecmp(buf,"s1\n") == 0 && n_s1 > 0)
						{
							n_s1--;
							printf("S1 allocated\n");
							write(logwrite.fd[j],"S1fifo",35);
						}
						else if(strcasecmp(buf,"s2\n") == 0 && n_s2 > 0)
						{
							n_s2--;
							printf("S2 allocated\n");
							write(logwrite.fd[j],"S2fifo",35);
						}
						else{
							printf("No servers avaliable\n");
						}
					}
					fflush(stdout);
				}
			}
		}
		fflush(stdout); 
	}
	return 0;
}