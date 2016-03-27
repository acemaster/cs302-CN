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

#define CONTROLLEN CSMG_LEN(sizeof(int))

static struct cmsghdr *cmptr = NULL;

int sendfd(int fd, int tosendfd)
{
	struct iovec iov[1];
	struct msghdr msg;
	char buf[2];
	iov[0].iov_base = buf;
	iov[0].iov_len = 2;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	if(tosendfd < 0)
	{
		msg.msg_control=NULL;
		msg.msg_controllen=0;
		buf[1] = -tosendfd;
		if(buf[1] == 0)
			buf[1] = 1;
	}
	else
	{
		if(cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return -1;
		cmptr->cmsg_level=SOL_SOCKET;
		cmptr->cmsg_type=SCM_RIGHTS;
		cmptr->cmsg_len = CONTROLLEN;
		msg.msg_control = cmptr;
		msg.msg_controllen=CONTROLLEN;
		*(int *)CMSG_DATA(cmptr) = tosendfd;
		buf[1]=0;
	}
	buf[0]=0;
	if(sendmsg(fd,&msg,0) !=2)
		return -1;
	return 0;
}


