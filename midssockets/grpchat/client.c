#include "vivek.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

struct services{
    int s1;
    int s2;
};

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        printf("Connection failed\n");
    while(1)
    {
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n > 0) 
        {
            portno=atoi(buffer);
            break;
        }
    }
    close(sockfd);
    printf("Recieved port: %d\n",portno);
    sleep(5);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("Connection failed\n");
    while(1){
        int pid=fork();
        if(pid == 0)
        {
            bzero(buffer,256);
            fgets(buffer,255,stdin);
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0) 
                 error("ERROR writing to socket");
            if(strcmp(buffer,"exit\n") ==0)
            {
                break;
            }
            bzero(buffer,256);
        }
        else{
            bzero(buffer,256);
            n = read(sockfd,buffer,255);
            if (n > 0) 
                printf("%s\n",buffer);
            bzero(buffer,256);
        }
    }
    return 0;
}