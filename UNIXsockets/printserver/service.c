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
	while(1) {
		fd = recvfd(usfd);
		if(fd < 0) {
			perror("recv_fd() ");
			exit(1);
		}
		printf("Number of bytes to print: ");
		scanf("%d", &n);
		read(fd, buf, n);
		printf("%s\n", buf);
		memset(buf, 0, n);
		if(sendfd(usfd, fd) < 0){
			perror("send_fd() ");
			exit(1);
		}
		close(fd);
	}
	return 0;
}