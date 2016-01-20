#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>


int main(int argc,char const *argv[])
{
	int n;
	sscanf(argv[1], "%d", &n);
	int a;
	char buf[10];
	while(n >0){
		int pid=fork();
		if(pid == 0){
			int fd1=open("input.txt",O_RDWR);
			int fd2=open("output.txt",O_RDWR);
			if(n%2 != 0)
			{
				a=dup2(fd2,1);
				if(read(fd1,&buf,5) == -1)
				{
					printf("Read error at phase %d\n",n);
				}
				printf("%s",buf);

			}
			else
			{
				a=dup2(fd1,1);
				if(read(fd2,&buf,5) == -1)
				{
					printf("Read error at phase %d\n",n);
				}
				printf("%s",buf);

			}
			close(fd1);
			close(fd2);
			exit(0); 
		}
		else if(pid > 0){
			int returnStatus; 
			waitpid(pid,&returnStatus, 0);
			printf("Process %d done\n",n);
			n--;
		}
    }
	return 0;
}


