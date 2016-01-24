/*Server goes here
 *
 * */

#include "vivek.h"
#include <poll.h>

int main(int argc,char *argv[]){
	printf("\nServer is starting................ Clients %d",argc);
	int *fd;
	int *writefd;
	int i;
	fd= malloc((argc-1)*sizeof(int));
	writefd=malloc((argc-1)*sizeof(int));
	for(i=1;i<argc;i++){
		char buf[50];
		if((fd[i-1]=open(argv[i],O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		snprintf(buf, sizeof(buf), "%s%s", argv[i], "write");
		if((writefd[i-1]=open(buf,O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		memset(buf,0,sizeof(buf));

	}
	struct pollfd fds[10];
	for(i=0;i<argc-1;i++)
	{
		fds[i].fd=fd[i];
		fds[i].events=POLLRDNORM;
	}
	int temp;
	int j=0;
	int k;
	printf("\nPoll fds are created................");
	printf("\nServer started..................");
	while(1){
		int retstatus=poll(fds,argc-1,2000);
		// printf("Polling\n");
		if(retstatus > 0){
			// printf("Got message\n");
			for(j=0;j<argc-1;j++)
			{
				if(fds[j].revents == fds[j].events){
					char buf[100];
					printf("User %d: ",j);
					if(read(fds[j].fd,buf,100) > 0)
					{
						printf("%s\n",buf);
					}

					for(k=0;k<argc-1;k++)
						if(k!=j)
							write(writefd[k],buf,sizeof(buf));
					fflush(stdout);
				}
			}
		}
		fflush(stdout); 
	}
	return 0;
}
			
		
	
	
