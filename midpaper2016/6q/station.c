#include "vivek.h"
#include <sys/select.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

struct services{
	int p[10];
	char *portno[10];
};


int main(int argc, char *argv[])
{
     int sfd[20],nsfd,n,portno[20];
     //==========Select config===============
     fd_set readset;
     FD_ZERO(&readset);
     struct timeval timeptr;
     timeptr.tv_sec=0;
     timeptr.tv_usec=0;
     int max=0;
     //==========End Select config===============
     socklen_t clientn;
     struct sockaddr_in myaddr,clientaddr;
     int i=0;
     //Stations ports
    for(i=0;i<argc-1;i++)
    {
        portno[i]=atoi(argv[i+1]);
        sfd[i]=socket(AF_INET,SOCK_STREAM,0);
         if(sfd[i]<0)
            error("Socket not initilizated");
        bzero((char *)&myaddr,sizeof(myaddr));
        myaddr.sin_family=AF_INET;
        myaddr.sin_addr.s_addr=INADDR_ANY;
        myaddr.sin_port=htons(portno[i]);
        if((bind(sfd[i],(struct sockaddr *) &myaddr, sizeof(myaddr)))<0)
            error("Error on binding: ");
        listen(sfd[i],5);
    }
    for(i=0;i<argc-1;i++)
    {
        FD_SET(sfd[i],&readset);
        if(sfd[i] > max)
            max=sfd[i];
    }
    int service=0;
    int shmid1;
    key_t key1;
    key1=300;
    int size=10000;
    struct services *shm;
    if ((shmid1 = shmget (key1, size, IPC_CREAT | 0666)) == -1) {
       perror("shmget: shmget failed"); exit(1); } else {
       (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid1);
    }
    shm = (struct services *)shmat(shmid1, NULL, 0);
    int startport=8005;
    for(i=0;i<2;i++)
    {
    	shm->p[i]=0;
        shm->portno[i]=(char *)malloc(100*sizeof(char));
    	snprintf(shm->portno[i],sizeof(shm->portno[i]),"%d",startport);
        printf("Port no of platform %d is %s\n",i,shm->portno[i]);
    	startport++;
    }
    FILE *platform1=popen("cd /home/acemaster/CS302/cs302-CN/midpaper2016/6q/;./platform 8005","w");
    FILE *platform2=popen("cd /home/acemaster/CS302/cs302-CN/midpaper2016/6q/;./platform 8006","w");
    int platfds[3];
    platfds[0]=fileno(platform1);
    platfds[1]=fileno(platform2);
    printf("\nSelect socket fds are created................");
    printf("\nServer started..................");
    clientn=sizeof(clientaddr);
    char buffer[250];
    bzero(buffer,250);
    int pid=1;
    int platform_no=-1;
    int platform_port=-1;
    int j;
    while(1 && pid > 0)
    {
        int retstatus=select(max+1,&readset,NULL,NULL,&timeptr);
        if(retstatus > 0)
        {
            // printf("Got message\n");
            for(i=0;i<argc-1;i++)
            {
                if(FD_ISSET(sfd[i],&readset))
                {
                    for(j=0;j<2;j++)
                    	if(shm->p[i]==0)
                    	{
                    		shm->p[i]=1;
                    		platform_no=i;
                            break;
                    	}
                    for(j=0;j<2;j++)
                        if(j!=platform_no)
                            write(platfds[j],"Got plaform",12);
                    nsfd=accept(sfd[i],(struct sockaddr *) &clientaddr, &clientn);
                    int pid=fork();
                    if(pid == 0){
                        write(nsfd,shm->portno[platform_no],sizeof(shm->portno[platform_no]));
                        close(nsfd);
                    }
                    else if(pid > 0){
                        close(nsfd);
                    }
                }
            }
        }
        fflush(stdout); 
        for(i=0;i<argc-1;i++)
        {
            FD_SET(sfd[i],&readset);
            if(sfd[i] > max)
                max=sfd[i];
        }
    }
}