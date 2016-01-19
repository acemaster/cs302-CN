#include <unistd.h>
#include <fcntl.h>

int main()
{
	char buf[]="Hello";
	int fd1=open("input.txt",O_RDWR);
	int fd2=open("output.txt",O_RDWR);
	printf("%d......%d\n",fd1,fd2);
	int a=dup2(fd1,fd2);
	printf("%d\n",a);
	if(write(fd1,&buf,4) == -1)
		printf("Error\n");
	close(fd1);
	close(fd2);
	return 0;
}


