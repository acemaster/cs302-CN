#include "vivek.h"



int main()
{
	int pfd[4];
	FILE *p2=popen("cd /home/acemaster/CS302/cs302-CN/midpaper2016/5q/;./p2","r");
	FILE *p5=popen("cd /home/acemaster/CS302/cs302-CN/midpaper2016/5q/;./p5","w");
	int wfd=fileno(p5);
	pfd[0]=fileno(p2);
	pfd[1]=open("hello.txt",O_RDONLY);
	dup2(pfd[1],1);
	struct pollfd fds[3];
	int i;
	for(i=0;i<2;i++)
	{
		fds[i].fd=pfd[i];
		fds[i].events=POLLRDNORM;
	}
	printf("\nPoll fds are created................");
	printf("\nServer started..................");
	char message[100];
	int flag=0;
	int j;
	while(1){
		int retstatus=poll(fds,3,2000);
		// printf("Polling\n");
		if(retstatus > 0){
			// printf("Got message\n");
			for(j=0;j<1;j++)
			{
				if(fds[j].revents == fds[j].events){
					flag=1;
					char buf[100];
					printf("User 1 %d: ",j);
					if(read(fds[j].fd,buf,100) > 0)
					{
						printf("%s\n",buf);
					}
					snprintf(message,sizeof(message),"User %d: %s",j,buf);
					write(wfd,message,sizeof(message));
					fflush(stdout);
					flag=0;
				}
			}
			if(flag == 0 && fds[1].revents == fds[1].events)
			{
				char buf[100];
					printf("User 2 %d: ",j);
					if(read(fds[j].fd,buf,100) > 0)
					{
						printf("%s\n",buf);
					}
					snprintf(message,sizeof(message),"User %d: %s",j,buf);
					write(wfd,message,sizeof(message));
					fflush(stdout);

			}
		}
		fflush(stdout); 
	}
	return 0;
}