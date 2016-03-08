#include "vivek.h"




int main()
{
	int n;
	char buffer[100];
	while(1)
	{
		n=read(0,buffer,sizeof(buffer));
		if(n>0)
		{
			printf("P6: %s",buffer);
			bzero(buffer,sizeof(buffer));
		}
	}
}