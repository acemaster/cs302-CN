#include "vivek.h"


pthread_mutex_t m;
pthread_mutex_t m2;
int threadno=0;
int sfd,n,portno;
struct sockaddr_in myaddr,clientaddr;
pthread_t *fthread;
socklen_t clientn;


struct queue{
	int front;
	int rear;
	int fdqueue[10];
	int length;
};


void push_q(struct queue *q,int fd)
{
	if(q->length < 10)
	{
		pthread_mutex_lock(m);
		q->fdqueue[(q->rear++)%10]=fd;
		q->length++;
		pthread_mutex_unlock(m);
	}
}


int pop(struct queue *q)
{
	if(q->length > 0)
	{
		pthread_mutex_lock(m2);
		int fd=q->fdqueue[(q->front++)%10]
		q->length--;
		pthread_mutex_unlock(m2);
		return fd;
	}
	else
		return -1;
}

void *threadacc(void *arg)
{
	while(1)
	{
		int nsfd;
		nsfd=accept(sfd,(struct sockaddr *)clientaddr,&clientn);
		printf("Started thread.........................\n");
	}
}

int main(int argc, char **argv)
{
	struct queue q;
	q.front=0;
	q.rear=0;
	q.length=0;
	portno=atoi(argv[1]);
	int nforks=atoi(argv[2])
	int nthread=atoi(argv[2]);
	fthread=malloc(nthread*nforks*sizeof(pthread));
	sfd=socket(AF_NET,SOCK_STREAM,0);
	bzero((char *)myaddr,sizeof(myaddr));
	myaddr.sin_family=AF_NET;
	myaddr.sin_addr.s_addr=INADDR_ANY;
	myaddr.sin_port=htons(portno);
	bind(&sfd,(struct sockaddr *)myaddr,sizeof(myaddr));
	listen(sfd,5);
	clientn=sizeof(clientaddr);
	int i;
	int j;
	for(i=0;i<nforks;i++)
	{
		int pid=fork();
		if(pid == 0)
		{
			for(j=0;j<nthread;j++)
			{
				pthread_create(&fthread[threadno],NULL,threadacc,NULL);
				threadno++;
			}
			break;
		}
		else
		{
			continue;
		}
	}
	int temp;
	for(i=0;i<threadno;i++)
		pthread_join(&fthread[i],&temp);


	return 0;
}