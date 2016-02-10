#include "vivek.h"
#include <pthread.h>

int fd;
pthread_t writethread;
void *temp;

void * thread_write(void *arg){
	char message[100];
	printf("\nKeep typing message\n");
	while(1)
	{
		printf("User: ");
		fgets(message,100,stdin);
		// printf("%s\n",message);
		write(fd,message,sizeof(message));
	}
}


int main(int argc, char *argv[])
{
	if(mkfifo(argv[1],0777) !=0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	char buf[50];
	fd=open(argv[1],O_RDWR);
	pthread_create(&writethread,NULL,thread_write,NULL);
	pthread_join(writethread,&temp);
	close(fd);

}
