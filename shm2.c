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
	int shm,shm2,s;
	key1=200;
	key2=100;
	size=10000;
	if ((shmid1 = shmget (key1, size, IPC_CREAT | 0666)) == -1) {
	   perror("shmget: shmget failed"); exit(1); } else {
	   (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid1);
	  }
	if ((shmid2 = shmget (key2, size, IPC_CREAT | 0666)) == -1) {
	   perror("shmget: shmget failed"); exit(1); } else {
	   (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid2);
	  }
	shm = (int)shmat(shmid1, NULL, 0);
    if(shm < 0)
    	shm=0;
    printf("Value recieved x: %d\n",shm);
    shm2 = (int)shmat(shmid2, NULL, 0);
    shm2=shm+1;
    printf("Value sent y: %d\n",shm2);
    signal(SIGUSR1,sig_usr);
    raise(SIGUSR1);
}

static void sig_usr(int signo)
{
if (signo == SIGUSR1){
		printf("received SIGUSR1\n");
		static char *argv[]={"file1",NULL};
        execv("file1",argv);
	}
}