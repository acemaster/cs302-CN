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
#include <sys/un.h>

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
	// printf("\n=====================CABLE TV======================\n");
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
	printf("\tAdvertisement: \n %s\n", buf);
}


//Unix sockets...........................

int init_sockconnectunix(const char * PATH) {
    struct sockaddr_un addr;
    int len, usfd;
    if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("socket() ");
        return -1;
    }
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, PATH);
    len = strlen(addr.sun_path) + sizeof(addr.sun_family);
    if (connect(usfd, (struct sockaddr *)&addr, len) == -1) {
        perror("connect() ");
        return -1;
    }
    return usfd;
}

int init_sockbindunix(const char * PATH) {
    struct sockaddr_un addr;
    int len, usfd;
    if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("socket() ");
        return -1;
    }
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, PATH);
    len = strlen(addr.sun_path) + sizeof(addr.sun_family);
    if (bind(usfd, (struct sockaddr *)&addr, len) == -1) {
        perror("bind() ");
        return -1;
    }
    if(listen(usfd, 5) < 0) {
        perror("listen() ");
        return -1;
    }
    return usfd;
}



static int sendfd(int socket, int fd)  // send fd by socket
{
    struct msghdr msg = { 0 };
    char buf[CMSG_SPACE(sizeof(fd))];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = "ABC", .iov_len = 3 };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

    *((int *) CMSG_DATA(cmsg)) = fd;

    msg.msg_controllen = cmsg->cmsg_len;

    if (sendmsg(socket, &msg, 0) < 0)
        return -1;
}

static int recvfd(int socket)  // receive fd from socket
{
    struct msghdr msg = {0};

    char m_buffer[256];
    struct iovec io = { .iov_base = m_buffer, .iov_len = sizeof(m_buffer) };
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    char c_buffer[256];
    msg.msg_control = c_buffer;
    msg.msg_controllen = sizeof(c_buffer);

    if (recvmsg(socket, &msg, 0) < 0)
        printf("Error\n");

    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);

    unsigned char * data = CMSG_DATA(cmsg);

    printf("Extracting fd.........\n");
    int fd = *((int*) data);
    printf("Extracted %d\n",fd);

    return fd;
}