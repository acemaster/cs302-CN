#include "vivek.h"



int main(int argc,char **argv)
{
	int pno=atoi(argv[1]);
	int sleept=atoi(argv[2]);
	fflush(stdout);
	int l=0;
	while(1)
	{
		fflush(stdout);
		sleep(sleept);
		printf("Vivek from Process %d : time %d\n",pno,l);
		l++;
	}
}