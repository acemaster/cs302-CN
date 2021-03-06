#include "vivek.h"

int total=0;

int main(int argc,char **argv)
{
	struct sockaddr_in serv_addr,saddr,daddr,checkaddr;
	int portno=atoi(argv[1]);
	int rsfd=init_sockbindsniff(portno);
	char buffer[BUFFER_SIZE];
	struct iphdr *iph;
	int recvbytes;
	int len;
	checkaddr.sin_addr.s_addr=inet_addr(argv[2]);
	while(1)
	{
		len=sizeof(serv_addr);
		if((recvbytes=recvfrom(rsfd,buffer,BUFFER_SIZE,0,(struct sockaddr *)&serv_addr,&len))<0)
		{
			printf("%d\n",recvbytes);
			perror("Send to ");
		}
		if(recvbytes > 0)
		{
			printf("Packet count: %d \n",total);
			iph=(struct iphdr *)(buffer+sizeof(struct ethhdr));
			printipheadersender(iph,saddr,daddr,buffer,checkaddr);
		}
		sleep(2);
	}
}