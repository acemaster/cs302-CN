#include "vivek.h"


int main(int argc, char **argv)
{
    unlink(argv[1]);
    int fd=open(argv[2],O_RDONLY);
    struct sockaddr_un remote; int len,n; char buf[256];
    if(fd < 0){
        perror("open() ");
        exit(1);
    }
    int usfd = init_sockbind(argv[1]);
    if(usfd < 0){
        printf("sock() error\n");
        exit(1);
    }
    len = sizeof(remote);
    int nusfd = accept(usfd, (struct sockaddr*)&remote, &len);  
    if(nusfd < 0) {
        perror("accept() ");
        exit(1);
    }
    while(1) {
        printf("Number of bytes to print: ");
        scanf("%d", &n);
        read(fd, buf, n);
        printf("%s\n", buf);
        memset(buf, 0, n);
        if(sendfd(nusfd, fd) < 0){
            perror("send_fd() ");
            exit(1);
        }
        close(fd);
        fd = recvfd(nusfd);
        if(fd < 0) {
            perror("recv_fd() ");
            exit(1);
        }
    }
    return 0;
}