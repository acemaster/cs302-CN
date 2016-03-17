#include "vivek.h"



struct nsfdcollection{
	int nsfd[100];
	int count;
};

struct nsfdcollection test;
pthread_t client[100];
pthread_mutex_t clientmut[100];


void *clientfunc(void *argv)
{
	printf("Starting thread................\n");
	int *id1=argv;
	int id=*(id1);
	int nsfd=test.nsfd[id];
	char buffer[100];
	int state=0;
	int target=0;
	int i;
	while(1)
	{
		printf("Checking for lock..........\n");
		pthread_mutex_lock(&clientmut[id]);
		int n=read(nsfd,buffer,sizeof(buffer));
		if(n > 0)
		{
			printf("Buffer: %s\n",buffer);
			if(strcmp(buffer,"send\n\n") == 0 && state == 0)
			{
				printf("Client %d sending....\n",id);
				for(i=0;i<test.count;i++)
					if(i != id)
						pthread_mutex_lock(&clientmut[i]);
				write(nsfd,"Client number",14);
				state=1;
			}
			else if(state == 1)
			{
				target=atoi(buffer);
				write(nsfd,"Client message",16);
				state=2;
			}
			else if(state ==2)
			{
				write(test.nsfd[target],buffer,sizeof(buffer));
				state=0;
				target=0;
				for(i=0;i<test.count;i++)
					if(i != id)
						pthread_mutex_unlock(&clientmut[i]);
			}
		}
		pthread_mutex_unlock(&clientmut[id]);
	}
}


int main(int argc,char **argv)
{
	printf("Starting server...............\n");
	test.count=0;
	socklen_t clientn;
    struct sockaddr_in myaddr,clientaddr;
    int portno=atoi(argv[1]);
    int sfd=socket(AF_INET,SOCK_STREAM,0);
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
    int service=0;
    while(1)
    {
    	printf("Creating thread...........\n");
        int nsfd=accept(sfd,(struct sockaddr *) &clientaddr, &clientn);
        int id=test.count;
        test.nsfd[id]=nsfd;
        pthread_mutex_init(&clientmut[id],NULL);
        pthread_create(&client[id],NULL,clientfunc,&id);
        test.count++;
    }
}