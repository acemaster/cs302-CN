#include "vivek.h"


int total=0;

int main(int argc,char **argv)
{
	struct sockaddr_in serv_addr,saddr,daddr;
	char buffer[BUFFER_SIZE],buf[128];
	int portno=atoi(argv[1]);
	int protocol=atoi(argv[2]);
	struct iphdr *iph;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	int rsfd=init_sockconnectrawhdrincl(portno,protocol);
	while(1)
	{
		printf("Type the message to send: \n");
		scanf("%s",buf);
		bzero(buffer,sizeof(buffer));
		iph = (struct iphdr *)buffer;
		iph->version = 4; /* IPv4 */
        iph->ihl = 5; /* default 5 */
        iph->tos = 0;
        iph->frag_off = 0;
        iph->ttl = 8;
        iph->protocol = 146;
        iph->saddr = inet_addr("127.0.1.1");
        iph->daddr = inet_addr("127.0.0.1");
        strcpy(buffer+sizeof(struct iphdr), buf);
        iph->tot_len = htons(sizeof(struct iphdr)+strlen(buf));
		if(sendto(rsfd,buffer,sizeof(buffer),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
		{
			perror("Recv from");
		}
		printf("Packet sent: %d",total++);
	}
}