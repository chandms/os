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


#define bool int
#define true 1
#define false 0

#define p(s)  semop(s,&pop,1)
#define v(s)  semop(s,&vop,1)
#define n 9
#define PORT 50026
#define PORT2 6756
#define BUFFSIZE 1024
bool rowCrossed(char board[1024])
{
    for (int i=0; i<3; i++)
    {
    	int u=i*3;
        if (board[u] == board[u+1] &&
            board[u+1] == board[u+2] && 
            board[u] != '-')
            return (true);
    }
    return(false);
}

bool columnCrossed(char board[1024])
{
    for (int i=0; i<3; i++)
    {

        if (board[i] == board[i+3] &&
            board[i+3] == board[i+6] && 
            board[i] != '-')
            return (true);
    }
    return(false);
}

bool diagonalCrossed(char board[1024])
{
    if (board[0] == board[4] &&
        board[4] == board[8] && 
        board[0] != '-')
        return(true);
         
    if (board[2] == board[4] &&
        board[4] == board[6] &&
         board[2] != '-')
        return(true);
 
    return(false);
}

bool check(char board[1024])
{
    return(rowCrossed(board) || columnCrossed(board)
            || diagonalCrossed(board) );
}

void f1()
{
	printf("Child creates socket\n");
}

void f2()
{
	printf("both starts working\n");
}


int main()
{

//////////////////////

int shm=shmget(1234,sizeof(int),0777|IPC_CREAT);
char *ar,*br,*e1,*e2;
int *sh,*se;
int *tu, *rn,*fr,*tr;
int shmid=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);
int id=shmget(2345,sizeof(int),0777|IPC_CREAT);
int idm=shmget(7273,sizeof(int),0777|IPC_CREAT);
int ndm=shmget(7273,sizeof(int),0777|IPC_CREAT);
printf("Shmid=%d\n",shmid);

int mc=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
int mp=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
int ne=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
int nf=semget(IPC_PRIVATE,1,0666|IPC_CREAT);

semctl(mc,0,SETVAL,1);
semctl(mp,0,SETVAL,1);
semctl(ne,0,SETVAL,1);
semctl(nf,0,SETVAL,0);

struct sembuf pop,vop;
pop.sem_num=0;
vop.sem_num=0;
pop.sem_flg=0;
vop.sem_flg=0;
pop.sem_op=-1;
vop.sem_op=1;

pid_t pid;
pid=fork();
if(pid==0)
{
	int socDes ,newsockfd;
	struct sockaddr_in thisAddr, thatAddr;
	int dataBytes, thatAddrLen ;
	signal(SIGUSR1,f1);
	printf("Child .........\n");
	ar=(char *)shmat(shmid,0,0);
	e1=(char *)shmat(ndm,0,0);
	tu=(int *)shmat(shm,0,0);
    sh=(int *)shmat(id,0,0);
    fr=(int *)shmat(idm,0,0);
    fr[0]=0;
    sh[0]=0;
	tu[0]=1;
	e1="0";
	pause();
	int dataSent;
	socDes = socket(AF_INET, SOCK_STREAM, 0);
	printf("child socDes=%d\n",socDes);
	if(socDes==-1) {
	perror("cannot create socket") ;
	return 0 ;
	}
	thisAddr.sin_family = AF_INET ;
	thisAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	thisAddr.sin_port = htons(PORT) ;

	if((bind(socDes, (struct sockaddr *)&thisAddr, sizeof(thisAddr))) < 0) {
	perror("cannot bind") ;
	return 0 ;
	}
	if (listen(socDes, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("listenning done in child\n");
    thatAddrLen = sizeof(thatAddr) ;
    socklen_t thatlen = sizeof(struct sockaddr_in);
	
	newsockfd=accept(socDes,(struct sockaddr*)&thatAddr,&thatlen);
	if(newsockfd<0)
	{
		perror("didn't accept\n");
		return 0;
	}
	printf("child:Accepted\n");

	kill(getppid(),SIGUSR1);
	for(int j=0;j<3;j++)
	{
		for(int k=0;k<3;k++)
		{
			int u=j*3+k;
			ar[u]='-';
		}
	}
	
	int fj=0;
	while(sh[0]!=9 && strcmp(e1,"#")!=0)
	{
		if(tu[0]==1)
		{
	        p(mp);
	        p(ne);
	        int pos;
	        printf("Child:Server: client1 enter the position:\n ");
	        char ds[100];
	        recv(newsockfd, ds, 1024, 0); 
	        pos=atoi(ds);
	        ar[pos]='X';
	        sh[0]++;
	        printf("C: After modification\n");
	        for(int r=0;r<3;r++)
	        {
	        	for(int e=0;e<3;e++)
	        	{
	        		int u=r*3+e;
	        		printf("%c ",ar[u]);
	        	}
	        	printf("\n");
	        }
	        bool w=check(ar);
	        if(w==1)
	        {
	        	fr[0]=100;
	        	printf("Child is winner \n");
	        	e1="#";
	        	//exit(0);
	        }
	        p(mc);
	        tu[0]=0;
	        v(mc);
	        v(ne);
	        v(mp);
	        
    	}
    	//send(newsockfd,e1,100,0);

	}
	if(sh[0]==9)
	{
	if(fr[0]==100)
	{
		e1="#";
		printf("Game is finished\n");
	}
	else
		printf("Match Draw\n");
	exit(1);
	}


}
else if(pid>0)
{
	rn=(int *)shmat(shm,0,0);
	rn[0]=0;
    tr=(int *)shmat(idm,0,0);
    tr[0]=0;
	int socDes ,newsockfd;
	struct sockaddr_in thisAddr, thatAddr;
	int dataBytes, thatAddrLen ;
	signal(SIGUSR1,f2);
	printf("Parent.........\n");
	br=(char *)shmat(shmid,0,0);
	e2=(char *)shmat(ndm,0,0);
	e2="0";
	int dataSent;
	se=(int *)shmat(id,0,0);
    se[0]=0;
	socDes = socket(AF_INET, SOCK_STREAM, 0);
	printf("parent socDes=%d\n",socDes);
	if(socDes==-1) {
	perror("cannot create socket") ;
	return 0 ;
	}
	thisAddr.sin_family = AF_INET ;
	thisAddr.sin_addr.s_addr = inet_addr("127.0.0.1") ; 
	thisAddr.sin_port = htons(PORT2) ;

	if((bind(socDes, (struct sockaddr *)&thisAddr, sizeof(thisAddr))) < 0) {
	perror("cannot bind") ;
	return 0 ;
	}
	if (listen(socDes, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("listenning done in parent\n");
	thatAddrLen = sizeof(thatAddr) ;
	socklen_t thatlen = sizeof(struct sockaddr_in);
	
	newsockfd=accept(socDes,(struct sockaddr*)&thatAddr,&thatlen);
	if(newsockfd<0)
	{
		perror("didn't accept\n");
		return 0;
	}
	printf("child:Accepted\n");


	kill(pid,SIGUSR1);
	pause();
	for(int j=0;j<3;j++)
	{
		for(int k=0;k<3;k++)
		{
			int u=j*3+k;
			br[u]='-';
		}
	}
	
	int fj=0;
	while(se[0]!=9 && strcmp(e2,"#")!=0)
	{
		if(rn[0]==0)
		{
        p(mp);
        p(ne);
        int pos;
	    printf("Parent:Server: client1 enter the position:\n ");
        char ds[100];
        recv(newsockfd, ds, 1024, 0); 
        pos=atoi(ds);
        br[pos]='O';
        se[0]++;
        for(int r=0;r<3;r++)
        {
        	for(int e=0;e<3;e++)
        	{
        		int u=r*3+e;
        		printf("%c ",br[u]);
        	}
        	printf("\n");
        }
        bool w=check(br);
	        if(w==1)
	        {
	        	tr[0]=100;
	        	printf("Parent is winner \n");
                e2="#";
	        	//exit(1);
	        }
        p(mc);
        rn[0]=1;
        v(mc);
        v(ne);
        v(mp);
        
    	}
    	//send(newsockfd,e2,100,0);
	}
	if(se[0]==9)
	{
	if(tr[0]==100)
	{
		e2="#";
		printf("Game is finished\n");
	}
	else
		printf("Match draw\n");
	exit(1);
	}



}


return 0;



}
