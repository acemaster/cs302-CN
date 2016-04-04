#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netdb.h>


int init_sockbindraw(int portno,int protocol,struct sockaddr_in &myaddr) {
    int len, sfd;
    if((sfd = socket(AF_INET, SOCK_RAW,protocol)) < 0){
        perror("socket() ");
        return -1;
    }
    myaddr.sin_family=AF_INET;
    myaddr.sin_addr.s_addr=INADDR_ANY;
    myaddr.sin_port=htons(portno);
    if((bind(sfd,(struct sockaddr *) &myaddr, sizeof(myaddr)))<0)
            error("Error on binding: ");
    return sfd;
}

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
//normal sockconnect for inet
int init_sockconnectraw(int portno,protocol) {
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int len, sfd;
    server = gethostbyname("acemaster");
    if((sfd = socket(AF_INET, SOCK_RAW, protocol)) < 0){
        perror("socket() ");
        return -1;
    }
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return -1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    return sfd;

}


void printipheader(struct iphdr *iphm, struct sockaddr_in &saddr,struct sockaddr_in &daddr,char *buffer)
{
	unsigned int iphdrlen;
	iphdrlen = iph->ihl*4;
	char buf[BUFFER_SIZE];
	printf("\n=====================IP HEADER======================\n")
	memset((char *)&s_addr, 0, sizeof s_addr);
	memset((char *)&d_addr, 0, sizeof d_addr);
	s_addr.sin_addr.s_addr = iph->saddr;
	d_addr.sin_addr.s_addr = iph->daddr;
	printf("|%4d|%4d|%8d|%16d|\n", (unsigned int)iph->version, (unsigned int)iph->ihl, (unsigned int)iph->tos, ntohs(iph->tot_len));
	printf("------------------------------------\n");
	printf("|%13d|R|D|M|%13d|\n", ntohs(iph->id), (unsigned int)iph->frag_off);
	printf("------------------------------------\n");
	printf("|%8d|%8d|%16d|\n", (unsigned int)iph->ttl, (unsigned int)iph->protocol, ntohs(iph->check));
	printf("------------------------------------\n");
	printf("%s\n", inet_ntoa(s_addr.sin_addr));
	printf("------------------------------------\n");
	printf("%s\n", inet_ntoa(d_addr.sin_addr));
	printf("------------------------------------\n");
	strcpy(buf, buffer+iphdrlen);
	printf("\tMessage\n %s\n", buf);
}