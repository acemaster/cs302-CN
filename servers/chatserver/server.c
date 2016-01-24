/*Server goes here
 *
 * */

#include "vivek.h"
#include <poll.h>

int main(int argc,char *argv[]){
	printf("\nServer is starting................ Clients %d",argc);
	int *fd;
	int i;
	fd= malloc((argc-1)*sizeof(int));
	for(i=1;i<argc;i++){
		if((fd[i-1]=open(argv[i],O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
	}
	struct pollfd fds[10];
	for(i=0;i<argc-1;i++)
	{
		fds[i].fd=fd[i];
		fds[i].events=POLLRDNORM;
	}
	int temp;
	int j=0;
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
					fflush(stdout);
				}
			}
		}
		fflush(stdout); 
	}
	return 0;
}
			
		
	
	
