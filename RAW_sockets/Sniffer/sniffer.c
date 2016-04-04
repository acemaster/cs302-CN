#include "vivek.h"

int total=0;

int main(int argc,char **argv)
{
	struct sockaddr_in serv_addr,saddr,daddr;
	int portno=atoi(argv[1]);
	int rsfd=init_sockbindsniff(portno);
	char buffer[BUFFER_SIZE];
	struct iphdr *iph;
	int recvbytes;
	int len;
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
			printipheader(iph,saddr,daddr,buffer);
		}
		sleep(2);
	}
}