#include "vivek.h"

int clientno=0;
int clients[10];
pthread_t clientthread[10];

struct clientthreadarg{
    int nsfd;
};


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

int main(int argc,char **argv)
{
	if(argc < 2) {
		printf("Usage: ./service <temperory_sock_path>\n");
		exit(1);
	}
	int usfd = init_sockconnect(argv[1]);
	if(usfd < 0){
		printf("sock() error\n");
		exit(1);
	}
	int fd,n; char buf[256];
	printf("Process 2 Started !!\n");
	int i=0;
	while(1) 
	{
		fd = recvfd(usfd);
		if(fd < 0) {
			perror("recv_fd() ");
			exit(1);
		}
		printf("Recieved client\n");
        struct clientthreadarg temp;
        temp.nsfd=fd;
        clients[clientno]=fd;
        pthread_create(&clientthread[clientno++],NULL,clientthreadfunc,&temp);
	}
	return 0;
}