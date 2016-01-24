#include "vivek.h"
#include <sys/wait.h>

/*
Popen directly opens a pipe to the file. So we can send input directly from stdout 
important commands here
- popen(cmd,r or w)
- 
*/
int main(){
	char line[MAXLINE];
	FILE *fpin;
	if((fpin = popen("cd /home/acemaster/CS302/cs302-CN/practice/;./myuclc","r"))== NULL)
		printf("Popen error\n");
	while(1){
		fputs("prompt>",stdout);
		fflush(stdout);
		if(fgets(line,MAXLINE,fpin) == NULL)
			break;
		if(fputs(line,stdout) == EOF)
			printf("Fputs to popen error\n");
	}
	if(pclose(fpin) == -1)
		printf("Error closing popen\n");
	exit(0);
}