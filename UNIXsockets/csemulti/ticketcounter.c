#include "vivek.h"

int acceptclients=1;

static void sig_usr(int signo);

int main(int argc, char **argv)
{
    signal(SIGUSR1,sig_usr);
    unlink("/tmp/socket1");
    struct sockaddr_un remote; int len,n; char buf[256];
    struct sockaddr_in client;
    int usfd = init_sockbind("/tmp/socket1");
    printf("In ticket counter child........ Creating socket for E counter\n");
    int usfd2 = init_sockbind("/tmp/socketE");
    if(usfd2 < 0)
    {
        printf("Sock() error.....\n");
        exit(1);
    }
    int sfd = init_sockbindinet(atoi(argv[1]));
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
    while(acceptclients) {
        int nsfd= accept(sfd, (struct sockaddr*)&client, &len);
        printf("Recieved client\n");
        if(sendfd(nusfd,nsfd) < 0)
            printf("Sending failed\n");
        close(nsfd);
    }
    return 0;
}


static void sig_usr(int signo)
{
    printf("Recieved signal.................\n");
    if(signo == SIGUSR1)
    {
        printf("Stopping the acceptance of clients\n");
        acceptclients=0;
    }
}