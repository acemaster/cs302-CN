#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <fcntl.h>
#include <signal.h>	
#include <pthread.h>
#include <poll.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>

#define BUFFER_SIZE 1024

int init_sockbindrawhdrincl(int portno,int protocol) {
    struct sockaddr_in myaddr;
    int len, sfd;
    if((sfd = socket(AF_INET, SOCK_RAW,protocol)) < 0){
        perror("socket() ");
        return -1;
    }
    int val=1;
    if(setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val)) < 0) {
        perror("setsockopt() ");
    }
    return sfd;
}


int init_sockconnectrawhdrincl(int portno,int protocol) {
    int len, sfd;
    if((sfd = socket(AF_INET, SOCK_RAW, protocol)) < 0){
        perror("socket() ");
        return -1;
    }
    int val=1;
    if(setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val)) < 0) {
        perror("setsockopt() ");
    }
    return sfd;

}

void printipheader(struct iphdr *iph, struct sockaddr_in s_addr,struct sockaddr_in d_addr,char *buffer)
{
	unsigned int iphdrlen;
	iphdrlen = iph->ihl*4;
	char buf[BUFFER_SIZE];
	printf("\n=====================CABLE TV======================\n");
	memset((char *)&s_addr, 0, sizeof s_addr);
	memset((char *)&d_addr, 0, sizeof d_addr);
	s_addr.sin_addr.s_addr = iph->saddr;
	d_addr.sin_addr.s_addr = iph->daddr;
	// printf("|%4d|%4d|%8d|%16d|\n", (unsigned int)iph->version, (unsigned int)iph->ihl, (unsigned int)iph->tos, ntohs(iph->tot_len));
	// printf("------------------------------------\n");
	// printf("|%13d|R|D|M|%13d|\n", ntohs(iph->id), (unsigned int)iph->frag_off);
	// printf("------------------------------------\n");
	// printf("|%8d|%8d|%16d|\n", (unsigned int)iph->ttl, (unsigned int)iph->protocol, ntohs(iph->check));
	// printf("------------------------------------\n");
	// printf("%s\n", inet_ntoa(s_addr.sin_addr));
	// printf("------------------------------------\n");
	// printf("%s\n", inet_ntoa(d_addr.sin_addr));
	// printf("------------------------------------\n");
	strcpy(buf, buffer+iphdrlen);
	printf("\tMessage\n %s\n", buf);
}