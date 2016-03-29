#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>

int init_sockconnect(const char * PATH) {
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

int init_sockbind(const char * PATH) {
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

    printf("Extracting.........\n");
    int fd = *((int*) data);
    printf("Extracted %d\n",fd);

    return fd;
}
