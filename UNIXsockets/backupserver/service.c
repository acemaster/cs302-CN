#include "vivek.h"


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
	return 0;
}