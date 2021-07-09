#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "sys/stat.h"

int jelek = 0;
pid_t mainszalertek = 0;
int messageQueue;


int rand_nev()
{
    
    return rand() % 4;
}

struct message
{
    long mtype;
    char mtext[1024];
};

void varakozas(int sig)
{
	if (sig == SIGUSR1)
	{
		jelek++;
	}
}
pid_t jatekos_egy(int pid_ch_rec,int pid_ch_out)
{
   
    int kerdes =0;
    pid_t szal = fork();
	if (szal == -1)
	{
		exit(-1);
	}
	if (szal > 0)
	{
		return szal;
	}
	kill(mainszalertek, SIGUSR1);
    read(pid_ch_rec,&kerdes,sizeof(int));
    printf("ok\n");
 switch (kerdes)
{
    case 1:
   
   
    break;
    case 2:
  
    break;
    case 3:
    ;
    break;
    case 4:
 
    break;
}
printf("ok\n");
    int status;
    struct message ms = {5, "negy"};
    status = msgsnd(messageQueue, &ms, strlen(ms.mtext) + 1, 0);
    if (status < 0)
    {
        perror("msgsnd");
    }
    printf("ok\n");
    exit(0);
}



int main(int argc, char **argv)
{
    signal(SIGUSR1, varakozas);
	mainszalertek = getpid();
  

    int status;
    key_t mainKey;

    mainKey = ftok(argv[0], 1);
    messageQueue = msgget(mainKey, 0600 | IPC_CREAT);
    if (messageQueue < 0)
    {
        perror("msgget");
        return 1;
    }

      int io_pipes2[2];
    int succ2 = pipe(io_pipes2);
    if (succ2 == -1)
        exit(-1);

    int io_pipes3[2];
    int succ3 = pipe(io_pipes3);
    if (succ3 == -1)
        exit(-1);



     int szam=0;

switch (rand_nev())
{
    case 1:
   szam=1;
    break;
    case 2:
    szam=2;
    break;
    case 3:
    szam=3;
    break;
    case 4:
    szam=4;
    break;
}
     jatekos_egy(io_pipes2[0], io_pipes3[1]);
    write(io_pipes2[1],&szam,sizeof(int));
	
    wait(NULL);
  
     while (jelek < 1);
	puts("Harcra fel ");
    struct message ms;
    status = msgrcv(messageQueue, &ms, 1024, 5, 0);
    if (status < 0)
    {
        perror("msgrcv");
    }
    else
    {
        sleep(0.3);
        printf("A kapott üzenet szovege:  %s \n", ms.mtype, ms.mtext);
    }
 
   printf("ok\n");
    close(io_pipes2[0]);
    close(io_pipes2[1]);
    close(io_pipes3[0]);
    close(io_pipes3[1]);
    status = msgctl(messageQueue, IPC_RMID, NULL);
    if (status < 0)
    {
        perror("msgctl");
    }
    return 0;
}