#include "vivek.h"

int main(int argc,char **argv)
{
	int threshold=atoi(argv[1]);
	printf("threshold is : %d\n",threshold);
	int usfd = init_sockconnect("/tmp/socket3");
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
			printf("threshold crossed ....... Closing ticket collection\n");
		}
	}
	return 0;
}