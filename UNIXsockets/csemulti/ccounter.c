#include "vivek.h"

int main(int argc,char **argv)
{
	int threshold=atoi(argv[1]);
	printf("threshold is : %d\n",threshold);
	int usfd = init_sockconnect("/tmp/socket1");
	struct sockaddr_un remote; int len;
    struct sockaddr_in client;
    int usfd2 = init_sockbind("/tmp/socket2");
	if(usfd < 0){
		printf("sock() error\n");
		exit(1);
	}
	if(usfd2 < 0){
        printf("sock() error\n");
        exit(1);
    }
    len = sizeof(remote);
    int nusfd = accept(usfd2, (struct sockaddr*)&remote, &len);  
	int fd,n; char buf[256];
	printf("Process 2 Started !!\n");
	int i=0;
	while(1) 
	{
		fd = recvfd(usfd);
		if(i<threshold)
		{
			if(fd < 0) {
				perror("recv_fd() ");
				exit(1);
			}
			int pid=fork();
			if(pid == 0)
			{
				while(1)
				{
					bzero(buf,sizeof(buf));
					n=read(fd,buf,100);
					if(n > 0)
					{
						printf("Client:%d %s\n",i,buf);
					}
				}
			}
			else
			{
				close(fd);
				i++;
			}
		}
		else
		{
			printf("threshold crossed ....... Transfering to S counter\n");
        	if(sendfd(nusfd,fd) < 0)
            	printf("Sending failed\n");
        	close(fd);
		}
	}
	return 0;
}