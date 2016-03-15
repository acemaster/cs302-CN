#include "vivek.h"

struct nsfdlist{
	int nsfd[100];
	int block[100];
	int nsfdcount;
	int nsfdsigcount;
};


int main(int argc, char **argv)
{
	int nsfdno=atoi(argv[1]);
	printf("Nsfd no is .........: %d\n",nsfdno);
	int shmid1;
    key_t key1;
    key1=400;
    int size=10000;
    struct nsfdlist *shm;
    if ((shmid1 = shmget (key1, size, IPC_CREAT | 0666)) == -1) {
       perror("shmget: shmget failed"); exit(1); } else {
       (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid1);
    }
    shm = (struct nsfdlist *)shmat(shmid1, NULL, 0);
	while(1)
	{
		fflush(stdout);
		char buffer[100];
		bzero(buffer,100);
		scanf("%s",buffer);
		int i=0;
		if(strcmp(buffer,"exit")==0)
			{
				shm->block[nsfdno]=0;
				break;
			}
		while(buffer[i])
		{
			buffer[i]=tolower(buffer[i]);
			i++;
		}
		printf("%s\n",buffer);
	}
}