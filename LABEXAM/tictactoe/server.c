#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define bool int
#define true 1
#define false 0


#define PORT 37827
#define PORT2 9382
#define p(s) semop(s,&pop,1)
#define v(s) semop(s,&vop,1)
void f1()
{
	printf("child gets \n");
	return ;
}
void f2()
{
	printf("parent gets \n");
	return;
}

bool chrow(char a[100])
{
	printf("Hi1\n");
	for(int j=0;j<3;j++)
	{
		int u=j*3;
		if(a[u]==a[u+1] && a[u+1]==a[u+2] && a[u]!='_')
			return 1;
	}
	return 0;
}

bool chcolumn(char a[100])
{
	printf("Hi2\n");
	for(int j=0;j<3;j++)
	{
		if(a[j]==a[j+3] && a[j+3]==a[j+6] && a[j]!='_')
			return 1;
	}
	return 0;
}

bool chdiagonal(char a[100])
{
	printf("Hi3\n");
	if(a[0]==a[4] && a[4]==a[8] && a[0]!='_')
		return 1;
	if(a[2]==a[4] && a[4]==a[6] && a[2]!='_')
		return 1;
	return 0;
}
bool check(char a[100])
{

	return(chrow(a) || chcolumn(a) || chdiagonal(a));
}
int main()
{

int shmid= shmget(1234,sizeof(int),0777|IPC_CREAT);
char *ar,*br;
pid_t pid;
pid= fork();
int mp=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
int mc=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
int ne=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
int nf=semget(IPC_PRIVATE,1,0666|IPC_CREAT);

semctl(mp,0,SETVAL,1);
semctl(mc,0,SETVAL,1);
semctl(ne,0,SETVAL,1);
semctl(nf,0,SETVAL,0);

struct sembuf pop,vop;
pop.sem_num=0;
vop.sem_num=0;
pop.sem_flg=0;
vop.sem_flg=0;
pop.sem_op=-1;
vop.sem_op=1;

int id=shmget(2332,sizeof(int),0777|IPC_CREAT);
int *sh,*sn;
if(pid==0)
{
	ar=(char *) shmat(shmid,0,0);
	sh=(int *)shmat(id,0,0);
	for(int j=0;j<3;j++)
	{
		for(int k=0;k<3;k++)
		{
			int u=3*j+k;
			ar[u]='_';
		}
	}
	sh[0]=0;
	printf("Child Process........................................\n");

	int sockDes,newsockfd;
	sockDes= socket(AF_INET,SOCK_STREAM,0);
	if(sockDes<0)
	{
		perror("Socket Creation problem\n");
		exit(0);
	}

	struct sockaddr_in thisAddr,thatAddr;

	thisAddr.sin_family= AF_INET;
	thisAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	thisAddr.sin_port=htons(PORT);

	int bd= bind(sockDes,(struct sockaddr *)&thisAddr,sizeof(thisAddr));
	if(bd<0)
	{
		perror("Binding problem\n");
		exit(0);
	}
     int thatlen= sizeof(thatAddr);
     listen(sockDes,1);
	newsockfd= accept(sockDes,(struct sockadddr *)&thatAddr,(socklen_t *)&thatlen);
	if(newsockfd<0)
	{
		perror("acceptance problem\n");
		exit(0);
	}
	printf("Client1 is accepted by child\n");

    while(sh[0]<9)
    {
    	signal(SIGUSR1,f1);

    	/*p(mp);
    	p(ne);*/
        
    	send(newsockfd,ar,100,0);
    	recv(newsockfd,ar,100,0);
    	for(int j=0;j<3;j++)
		{
		for(int k=0;k<3;k++)
		{
			int u=j*3+k;
			printf("%c ",ar[u]);
		}
		printf("\n");
		}
		sh[0]++;
		int ch=check(ar);
		printf("cch = %d\n",ch);
		if(ch==1)
		{
			printf("Client1 has own\n");
			kill(getppid(),SIGKILL);
			sh[0]=50;
		}
    	/*v(ne);
    	v(mp);*/
    	kill(getppid(),SIGUSR1);
    	printf("Waiting for SIGUSR to come.....child\n");
    	pause();
    	



    }
    if(sh[0]!=50)
    {
    	printf("Draw\n");
    	kill(getppid(),SIGKILL);
    }

}

else if(pid>0)
{
   br=(char *) shmat(shmid,0,0);
   sn=(int *)shmat(id,0,0);
	printf("Parent Process........................................\n");

	int sockDes,newsockfd;
	sockDes= socket(AF_INET,SOCK_STREAM,0);
	if(sockDes<0)
	{
		perror("Socket Creation problem\n");
		exit(0);
	}

	struct sockaddr_in thisAddr,thatAddr;

	thisAddr.sin_family= AF_INET;
	thisAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	thisAddr.sin_port=htons(PORT2);

	int bd= bind(sockDes,(struct sockaddr *)&thisAddr,sizeof(thisAddr));
	if(bd<0)
	{
		perror("Binding problem\n");
		exit(0);
	}
     int thatlen= sizeof(thatAddr);
     listen(sockDes,1);
	newsockfd= accept(sockDes,(struct sockadddr *)&thatAddr,(socklen_t *)&thatlen);
	if(newsockfd<0)
	{
		perror("acceptance problem\n");
		exit(0);
	}
	printf("Client2 is accepted by parent\n");
	while(sn[0]<9)
    {
    	signal(SIGUSR1,f2);
    	printf("Waiting for SIGUSR to come.....parent\n");
    	pause();
    	//p(mp);
    	//p(ne);
    	send(newsockfd,br,100,0);
    	recv(newsockfd,br,100,0);
    	for(int j=0;j<3;j++)
		{
		for(int k=0;k<3;k++)
		{
			int u=j*3+k;
			printf("%c ",br[u]);
		}
		printf("\n");
		}

    	//v(ne);
    	//v(mp);
    	sn[0]++;
    	int ch=check(br);
    	printf("pch = %d\n",ch);
		if(ch==1)
		{
			printf("Client2 has own\n");
			kill(pid,SIGKILL);
			sn[0]=50;
		}
    	kill(pid,SIGUSR1);



    }
    if(sn[0]!=50)
    {
    	printf("Draw\n");
    	kill(pid,SIGKILL);
    }

}

return 0;

}