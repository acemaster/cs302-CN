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


struct nsfdlist{
	int nsfd[100];
	int block[100];
	int nsfdcount;
	int nsfdsigcount;
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

struct nsfdlist list;
int nsfdno=0;
int nsfdsigno=0;
char *lower_args[] = {"./lower", "nsfdno", NULL};
struct nsfdlist *shm;


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
	if(mkfifo("mainfifo",0777) !=0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	if((logs.fd[0]=open("mainfifo",O_RDWR)) < 0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	if(mkfifo("mainfifo2",0777) !=0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	if((logs.fd[1]=open("mainfifo2",O_RDWR)) < 0)
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
	FD_SET(0,&readset);
	int pid;
	//Socket config
	int sfd,nsfd,n,portno;
     socklen_t clientn;
     char buffer[100];
     struct sockaddr_in myaddr,clientaddr;
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
    printf("Socket creation done......................\n");
    //Config done
    FD_SET(sfd,&readset);
    if(max < sfd)
    	max=sfd;
    //Shared memory init

    int shmid1;
    key_t key1;
    key1=400;
    int size=10000;
    if ((shmid1 = shmget (key1, size, IPC_CREAT | 0666)) == -1) {
       perror("shmget: shmget failed"); exit(1); } else {
       (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid1);
    }
    shm = (struct nsfdlist *)shmat(shmid1, NULL, 0);
    shm->nsfdcount=0;
    shm->nsfdsigcount=0;
    printf("SHM creation done...................\n");
    //Ending init
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
					nsfd=accept(sfd,(struct sockaddr *) &clientaddr, &clientn);
					printf("Got Client socket..............\n");
					int nsfdid=shm->nsfdcount;
					shm->nsfd[nsfdid]=nsfd;
					shm->block[nsfdid]=0;
					shm->nsfdcount++;
					printf("Done adding to shared mem:::\n");
					int pid=fork();
					if(pid == 0)
					{
						close(sfd);
						printf("NSFDId: %d\n",nsfdid);
			            while(1){
			                bzero(buffer,250);
			                n=read(nsfd,buffer,100);
			                if(n > 0 && shm->block[nsfdid]==0)
			                {
			                	if(strcmp(buffer,"exit\n") == 0)
                                {
                                    close(nsfd);
                                    break;
                                }
                                printf("Client: %s\n",buffer);
                                buffer[0]=toupper(buffer[0]);
                                n=write(nsfd,buffer,sizeof(buffer));
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
			FD_SET(0,&readset);
			FD_SET(sfd,&readset);
		    if(max < sfd)
		    	max=sfd;
		}
	}
	else{
			char message[]="This is from pipe";
			write(logs.pipefd[1],message,sizeof(message));
			signal(SIGUSR1,sig_usr);
			printf("Binded signal\n");
			while(1){};
	}
	return 0;
}


static void sig_usr(int signo)
{
	printf("Raised signal\n");
	if (signo == SIGUSR1){
		printf("\nreceived SIGUSR1\n");
		int pid=fork();
		if(pid == 0)
		{
		 int nsfd=shm->nsfd[shm->nsfdsigcount];
		 shm->block[shm->nsfdsigcount++]=1;
		 char buffer[100];
		 snprintf(buffer,sizeof(buffer),"%d",shm->nsfdsigcount-1);
		 printf("\n");
		 strcpy(lower_args[1],buffer);
		 bzero(buffer,250);
         dup2(nsfd,0);
       	 dup2(nsfd,1);
       	 printf("Exceing lower\n");
         execv("./lower",lower_args);
         printf("Failed\n");
       	}
	}
}