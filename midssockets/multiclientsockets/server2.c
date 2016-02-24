#include "vivek.h"

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <sys/select.h>

pthread_t clientthread[10];
int clientno=0;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

struct clientthreadarg{
	int nsfd;
};

void *clientthreadfunc(void *arg)
{
	char buffer[250];
	struct clientthreadarg *argv=arg;
	int nsfd=argv->nsfd;
	int n;
	while(1){
        bzero(buffer,250);
        n=read(nsfd,buffer,100);
        if(n > 0)
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
    printf("Client leaving...........\n");
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
     int sfd[20],nsfd,n,portno[20];
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
    printf("\nStarted mini server................");
    printf("\nMini server running..................");
    clientn=sizeof(clientaddr);
    char buffer[250];
    bzero(buffer,250);
    int pid=1;
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
                    nsfd=accept(sfd[i],(struct sockaddr *) &clientaddr, &clientn);
                    struct clientthreadarg temp;
                    temp.nsfd=nsfd;
                    pthread_create(&clientthread[clientno++],NULL,clientthreadfunc,&temp);
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