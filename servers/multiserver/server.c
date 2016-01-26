#include "vivek.h"
#include <pthread.h>

int fd;
int writefd;
pthread_t writethread,readthread;
void *temp;
int gotserver=0;

struct logfds{
	int number;
	int fd[10];
};

void * thread_write(void *arg){
	char message[100];
	printf("Which server S1 or S2:\n");
	fgets(message,100,stdin);
	printf("%s\n",message);
	write(fd,message,sizeof(message));
	while(1){
		if(gotserver == 1){
				printf("Send a message:\n");
				fgets(message,100,stdin);
				printf("%s\n",message);
				write(fd,message,sizeof(message));
		}
	}
}

void * thread_read(void *arg){
	char buf[100];
	while(1){
		if(read(writefd,buf,100) > 0)
		{
				printf("User: ");
				printf("%s\n",buf);
		}
	}
}

int main(int argc, char *argv[])
{
	char buf[100];
	char buf2[100];
	snprintf(buf2,sizeof(buf2),"%s%s",argv[1],"write");

	//Main Server opening	
	fd=open(argv[1],O_RDWR);
	writefd=open(buf2,O_RDWR);
	// pthread_create(&writethread,NULL,thread_write,NULL);
	pthread_create(&readthread,NULL,thread_read,NULL);
	// pthread_join(writethread,&temp);
	pthread_join(readthread,&temp);
	close(fd);
	close(writefd);

}
