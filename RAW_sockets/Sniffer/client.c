#include "vivek.h"

#define BUFFER_SIZE 1024

int total=0;

int main(int argc,char **argv)
{
	struct sockaddr_in serv_addr,saddr,daddr;
	int portno=atoi(argv[1]);
	int protocol=atoi(argv[2]);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	int rsfd=init_sockbindraw(portno,protocol,serv_addr);
	char buffer[BUFFER_SIZE];
	struct iphdr *iph;
	while(1)
	{
		if(recvfrom(rsfd,BUFFER_SIZE,0,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
		{
			perror("Send to ");
		}
		printf("Packet count: %d \n",total);
		iph=(struct iphdr *)buffer;
		printipheader(iph,saddr,daddr,buffer);
	}
}