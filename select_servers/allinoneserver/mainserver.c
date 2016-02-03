#include "vivek.h"
#include <poll.h>

static void sig_usr(int signo);

struct logfds{
	int number;
	int fd[10];
	int pipefd[2];
	int totalnumber;
};


/*
	The clients are as follows:
		1. Keyboard :Done
		2. Pipe fd :Done
		3. Fifo fd: Done
		4. Popen
		5. Signal : Done
*/

//All in one Server
int main(int argc,char *argv[]){
	printf("\nMain Server is starting................");
	int fd;
	struct logfds logs;
	struct logfds logwrite;
	int i;
	dup2(logs.fd[0],0);
	if((logs.fd[1]=open("mainfifo",O_RDWR)) < 0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	if(mkfifo("mainfifo2",0777) !=0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	if((logs.fd[2]=open("mainfifo2",O_RDWR)) < 0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	pipe(logs.pipefd);
	logs.number=3;
	struct pollfd fds[10];
	for(i=0;i<logs.number;i++){
		fds[i].fd=logs.fd[i];
		fds[i].events=POLLRDNORM;
	}
	fds[logs.number].fd=logs.pipefd[0];
	fds[logs.number].events=POLLRDNORM;
	int pid;
	if((pid = fork())<0)
		printf("Error for fork");
	else if(pid > 0)
	{
		int temp;
		int j=0;
		int k;
		printf("\nClient fds are created................");
		printf("\nServer started..................");
		while(1){
			int retstatus=poll(fds,logs.number+1,2000);
			// printf("Polling\n");
			if(retstatus > 0){
				// printf("Got message\n");
				for(j=0;j<logs.number+1;j++)
				{
					if(fds[j].revents == fds[j].events){
						char buf[100];
						printf("Logged: ");
						if(read(fds[j].fd,buf,100) > 0)
						{
							printf("\n%s\n",buf);
						}
						fflush(stdout);
					}
				}
			}
			fflush(stdout); 
		}
	}
	else{
			char message[]="This is from pipe";
			write(logs.pipefd[1],message,sizeof(message));
			signal(SIGUSR1,sig_usr);
    		raise(SIGUSR1);
	}
	return 0;
}


static void sig_usr(int signo)
{
	if (signo == SIGUSR1){
		printf("\nreceived SIGUSR1\n");
		int fd=open("mainfifo2",O_RDWR);
		char message[]="got this from signal";
		write(fd,message,sizeof(message));	
	}
}