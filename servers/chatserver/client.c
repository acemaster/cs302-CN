/*client goes here*/
/*
#include <stropts.h>
#include <poll.h>
...
struct pollfd fds[2];
int timeout_msecs = 500;
int ret;
    int i;

fds[0].fd = open("/dev/dev0", ...);
fds[1].fd = open("/dev/dev1", ...);
    fds[0].events = POLLOUT;
    fds[1].events = POLLOUT;

ret = poll(fds, 2, timeout_msecs);

if (ret > 0) {
    for (i=0; i<2; i++) {
        if (fds[i].revents & POLLOUT) {
        
        }
        
    }
} */


#include "vivek.h"
int main(int argc, char *argv[])
{
	if(mkfifo(argv[1],0777) !=0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	int fd=open(argv[1],O_RDWR);
	char message[10];
	printf("\nKeep typing message\n");
	while(1)
	{
		printf("%s: ",argv[1]);
		scanf("%s",message);
		printf("%s\n",message);
		write(fd,message,sizeof(message));
	}	
	close(fd);
}
