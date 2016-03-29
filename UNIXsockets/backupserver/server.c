#include "vivek.h"
int clients[2];
pthread_t clientthread[2];

struct clientthreadarg{
    int nsfd;
};

int clientno=0;

void *clientthreadfunc(void *arg)
{
    printf("In thread...........\n");
    struct clientthreadarg *argv=arg;
    int nsfd=argv->nsfd;
    int n;
    int i;
    char buffer[250];
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
            printf("Server: %s\n",buffer);
        }
    }
    printf("Client leaving...........\n");
    pthread_exit(0);
}


int main(int argc, char **argv)
{
    int threshold=2;
    unlink(argv[1]);
    struct sockaddr_un remote; int len,n; char buf[256];
    struct sockaddr_in client;
    int usfd = init_sockbind(argv[1]);
    int sfd = init_sockbindinet(atoi(argv[2]));
    if(usfd < 0){
        printf("sock() error\n");
        exit(1);
    }
    if(sfd < 0)
    {
        printf("Inet sock() error\n");
        exit(1);
    }
    len = sizeof(remote);
    int nusfd = accept(usfd, (struct sockaddr*)&remote, &len);  
    if(nusfd < 0) {
        perror("accept() ");
        exit(1);
    }
    while(1) {
        int nsfd= accept(sfd, (struct sockaddr*)&client, &len);
        if(clientno<threshold)
        {
            printf("Recieved client\n");
            struct clientthreadarg temp;
            temp.nsfd=nsfd;
            clients[clientno]=nsfd;
            pthread_create(&clientthread[clientno++],NULL,clientthreadfunc,&temp);
        }
        else
        {
            printf("Threshold crossed........Transfering clients to backup server\n");
            int i=0;
            for(i=0;i<clientno;i++)
            {
                pthread_cancel(clientthread[i]);
                if(sendfd(nusfd,clients[i]) < 0)
                    printf("Sending failed\n");
                close(clients[i]);
            }
            clientno=0;
            struct clientthreadarg temp;
            temp.nsfd=nsfd;
            clients[clientno]=nsfd;
            pthread_create(&clientthread[clientno++],NULL,clientthreadfunc,&temp);
        }
    }
    return 0;
}