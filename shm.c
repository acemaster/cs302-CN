#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>

static void sig_usr(int signo);

int main(){
	key_t key1,key2;
	int shmflg;
	int shmid1,shmid2;
	int size;
	char *shm,*shm2,*s;
	key1=100;
	key2=200;
	size=10000;
	if ((shmid1 = shmget (key1, size, IPC_CREAT | 0666)) == -1) {
	   perror("shmget: shmget failed"); exit(1); } else {
	   (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid1);
	   exit(0);
	  }
	if ((shmid2 = shmget (key2, size, IPC_CREAT | 0666)) == -1) {
	   perror("shmget: shmget failed"); exit(1); } else {
	   (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid2);
	   exit(0);
	  }
	if ((shm = shmat(shmid1, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    int flag=0;
    for(*s=shm;s!=NULL;s++)
    {
    	flag=1;
    }
    if(flag == 0)
    	*shm='a';
    if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    *shm2=*shm+1;
    signal(SIGUSR1,sig_usr);
    raise(SIGUSR1);
}



static void sig_usr(int signo)
{
	if (signo == SIGUSR1){
			printf("received SIGUSR1\n");
			static char *argv[]={"file2",NULL};
	        execv("file2",argv);
		}
}