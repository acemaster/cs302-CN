#include "vivek.h"


int total=0;

int main(int argc,char **argv)
{
	struct sockaddr_in serv_addr,saddr,daddr;
	int portno=atoi(argv[1]);
	int protocol=atoi(argv[2]);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	int rsfd=init_sockconnectraw(portno,protocol,serv_addr);
	while(1)
	{
		if(sendto(rsfd,"Vivek packet",15,0,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
		{
			perror("Recv from");
		}
		printf("Packet sent: %d",total++);
		sleep(3);
	}
}