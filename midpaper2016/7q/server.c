#include "vivek.h"
#include <poll.h>
#include <sys/select.h>

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
	fd_set readset;
	FD_ZERO(&readset);
	struct timeval timeptr;
	timeptr.tv_sec=0;
	timeptr.tv_usec=0;
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
	int max=0;
	for(i=0;i<logs.number;i++){
		FD_SET(logs.fd[i],&readset);
		if(max < logs.fd[i])
			max=logs.fd[i];
	}
	FD_SET(logs.pipefd[0],&readset);
	if(max < logs.pipefd[0])
		max=logs.pipefd[0];
	int pid;
	//Socket config
	int sfd,nsfd,n,portno;
     socklen_t clientn;
     char buffer[100];
     struct sockaddr_in myaddr,clientaddr;
     int i=0;
    portno=atoi(argv[1]);
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd<0)
        error("Socket not initilizated");
    bzero((char *)&myaddr,sizeof(myaddr));
    myaddr.sin_family=AF_INET;
    myaddr.sin_addr.s_addr=INADDR_ANY;
    myaddr.sin_port=htons(portno);
    if((bind(sfd,(struct sockaddr *) &myaddr, sizeof(myaddr)))<0)
            error("Error on binding: ");
    listen(sfd,5);
    //Config done
    FD_SET(sfd,&readset);
    if(max < sfd)
    	max=sfd;
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
			int retstatus=select(max+1,&readset,NULL,NULL,&timeptr);
			// printf("Polling\n");
			if(retstatus > 0){
				// printf("Got message\n");
				for(j=0;j<logs.number;j++)
				{
					if(FD_ISSET(logs.fd[j],&readset))
					{
						char buf[100];
						printf("Logged: ");
						if(read(logs.fd[j],buf,100) > 0)
						{
							printf("\n%s\n",buf);
						}
						fflush(stdout);
					}
				}
				if(FD_ISSET(logs.pipefd[0],&readset))
				{
					char buf[100];
					printf("Logged: ");
					if(read(logs.pipefd[0],buf,100) > 0)
					{
						printf("\n%s\n",buf);
					}
					fflush(stdout);
				}
				if(FD_ISSET(sfd,&readset))
				{
					int pid=fork();
					nsfd=accept(sfd,(struct sockaddr *) &clientaddr, &clientn);
					if(pid == 0)
					{
						close(sfd);
			            while(1){
			                bzero(buffer,250);
			                n=read(nsfd,buffer,100);
			                if(n > 0)
			                {
			                    if(strcmp(buffer,"exit\n") == 0)
			                    {
			                        for(i=0;i<2;i++)
			                            if(strcmp(shm->portno[i],argv[1]) == 0)
			                                    shm->p[i]=0;
			                        close(nsfd);
			                        break;
			                    }
			                    printf("Train: %s\n",buffer);
                			}
            			}
					}
					else{
						close(nsfd);
					}
				}
			}
			fflush(stdout); 
			for(i=0;i<logs.number;i++)
			{
				FD_SET(logs.fd[i],&readset);
				if(max < logs.fd[i])
					max=logs.fd[i];
			}
			FD_SET(logs.pipefd[0],&readset);
			if(max < logs.pipefd[0])
				max=logs.pipefd[0];
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