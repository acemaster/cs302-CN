#include "vivek.h"

int main(){
	int n;
	int fd[2];
	pid_t pid;
	char line[MAXLINE];
	if(pipe(fd)<0)
		printf("Pipe error");
	if((pid = fork()) < 0)
		printf("Fork error");
	else if(pid > 0)
	{
		close(fd[0]);
		write(fd[1],"Hello world",12);
	}
	else{
		close(fd[1]);
		n=read(fd[0],line,MAXLINE);
		write(STDOUT_FILENO,line,n);
	}
	exit(0);
}