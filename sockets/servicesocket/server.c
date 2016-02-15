/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include "vivek.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sfd,nsfd,n,portno;
     socklen_t clientn;
     struct sockaddr_in myaddr,clientaddr;
     portno=atoi(argv[1]);
     sfd=socket(AF_INET,SOCK_STREAM,0);
     if(sfd<0)
        error("Socket not initilizated");
    bzero((char *)&myaddr,sizeof(myaddr));
    myaddr.sin_family=AF_INET;
    myaddr.sin_addr.s_addr=INADDR_ANY;
    myaddr.sin_port=htons(portno);
    if((bind(sfd,(struct sockaddr *) &myaddr, sizeof(myaddr)))<0)
        error("Error on binding: ");
    listen(sfd,5);
    clientn=sizeof(clientaddr);
    char buffer[250];
    bzero(buffer,250);
    int pid=1;
    while(1 && pid > 0)
    {
        nsfd=accept(sfd,(struct sockaddr *) &clientaddr, &clientn);
        int pid=fork();
        if(pid == 0){
            close(sfd);
            while(1){
                bzero(buffer,250);
                n=read(nsfd,buffer,100);
                if(n > 0)
                {
                    if(strcmp(buffer,"exit\n") == 0)
                    {
                        close(nsfd);
                        break;
                    }
                    printf("Client: %s\n",buffer);
                    buffer[0]=toupper(buffer[0]);
                    n=write(nsfd,buffer,sizeof(buffer));
                }
            }
        }
        else if(pid > 0){
            close(nsfd);
            continue;
        }
    }
}