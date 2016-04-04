#include "vivek.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

struct services{
    int p[10];
    int portno[10];
};


pthread_t cabletv;

struct clientthreadarg{
    int portno;
    int protocol;
};

void *clientthreadfunc(void *arg)
{
    int total=0;
    struct sockaddr_in serv_addr,saddr,daddr;
    struct clientthreadarg *argv=arg;
    int portno=argv->portno;
    int protocol=argv->protocol;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    int rsfd=init_sockbindrawhdrincl(portno,protocol);
    char buffer[BUFFER_SIZE];
    struct iphdr *iph;
    int recvbytes;
    int len;
    while(1)
    {
        len==sizeof(serv_addr);
        if((recvbytes=recvfrom(rsfd,buffer,BUFFER_SIZE,0,(struct sockaddr *)&serv_addr,&len))<0)
        {
            printf("%d\n",recvbytes);
            perror("Send to ");
        }
        if(recvbytes > 0)
        {
            printf("Message count: %d \n",total++);
            iph=(struct iphdr *)buffer;
            printipheader(iph,saddr,daddr,buffer);
        }
    }
}


int main(int argc, char *argv[])
{
    int parentid=atoi(argv[2]);
     int sfd,nsfd,n,portno;
     socklen_t clientn;
     char buffer[100];
     struct clientthreadarg temp;
    temp.portno=atoi(argv[3]);
    temp.protocol=146;
    pthread_create(&cabletv,NULL,clientthreadfunc,&temp);
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
    int size=1000;
    struct services *shm;
    if ((shmid1 = shmget (key1, size, IPC_CREAT | 0666)) == -1) {
       perror("shmget: shmget failed"); exit(1); } else {
       (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid1);
    }
    if((shm = (struct services *)shmat(shmid1, NULL, 0))<0)
    {
        perror("Failed to shmat:");
        exit(1);
    }
    for(i=0;i<2;i++)
        printf("SHM port value: %d\t SHM platform status: %d",shm->portno[i],shm->p[i]);

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
                        printf("Exiting...............\n");
                        for(i=0;i<2;i++)
                            if(shm->portno[i]== portno)
                                    shm->p[i]=0;
                        close(nsfd);
                         printf("Exiting......sending signal to %d\n",parentid);
                        kill(parentid,SIGUSR1);
                        exit(0);
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