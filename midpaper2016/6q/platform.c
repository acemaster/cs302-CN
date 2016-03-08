#include "vivek.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

struct services{
    int p[10];
    char *portno[10];
};

int main(int argc, char *argv[])
{
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
    while(1)
    {
        nsfd=accept(sfd,(struct sockaddr *) &clientaddr, &clientn);
        int pid=fork();
        if(pid == 0){
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
        else if(pid > 0){
            close(nsfd);
            continue;
        }
    }

}