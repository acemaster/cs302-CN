#include "vivek.h"


int main(int argc, char *argv[])
{
    int sockfd=init_sockconnectinet(atoi(argv[1]));
    char buffer[256];
    int n;
    while(1){
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             perror("ERROR writing to socket");
        if(strcmp(buffer,"exit\n") ==0)
            break;
        bzero(buffer,256);
    }
    close(sockfd);
    return 0;
}