#include "vivek.h"

int total=0;

int main(int argc,char **argv)
{
	struct sockaddr_in serv_addr,saddr,daddr;
	int portno=atoi(argv[1]);
	int protocol=atoi(argv[2]);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	int rsfd=init_sockbindrawhdrincl(portno,protocol);
	char buffer[BUFFER_SIZE];
	struct iphdr *iph;
	int recvbytes;
	int len;
	while(1)
	{
		len==sizeof(serv_addr);
		if((recvbytes=recvfrom(rsfd,buffer,BUFFER_SIZE,0,(struct sockaddr *)&serv_addr,&len))<0)
		{
			printf("%d\n",recvbytes);
			perror("Send to ");
		}
		if(recvbytes > 0)
		{
			printf("Packet count: %d \n",total);
			iph=(struct iphdr *)buffer;
			printipheader(iph,saddr,daddr,buffer);
		}
	}
}