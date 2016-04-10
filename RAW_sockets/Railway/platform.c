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
    int platform_no=atoi(argv[4]);
     int sfd,nsfd,n,portno;
     socklen_t clientn;
     char buffer[100];
     struct clientthreadarg temp;
    temp.portno=atoi(argv[3]);
    temp.protocol=146;
    pthread_create(&cabletv,NULL,clientthreadfunc,&temp);
    int usfd = init_sockconnectunix(argv[1]);
    if(usfd < 0){
        printf("sock() error\n");
        exit(1);
    }
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
    int i;
    /*for(i=0;i<2;i++)
        printf("SHM port value: %d\t SHM platform status: %d",shm->portno[i],shm->p[i]);*/

    while(1)
    {
        nsfd = recvfd(usfd);
        /*printf("Recieved fd: \n");*/
        if(nsfd < 0) {
            perror("recv_fd() ");
            exit(1);
        }
        int pid=fork();
        if(pid == 0){
            while(1){
                bzero(buffer,250);
                n=read(nsfd,buffer,100);
                if(n > 0)
                {
                    if(strcmp(buffer,"exit\n") == 0)
                    {
                        printf("Exiting...............\n");
                        shm->p[platform_no]=0;
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