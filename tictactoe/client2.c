#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // Internet family of protocols
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <signal.h>

#define SERVERIP "127.0.0.1"
#define PORT 6756
#define BUFFSIZE 1024
int main(){
int socDes ;
struct sockaddr_in thatAddr ;
char buff[BUFFSIZE] = "NIT DURGAPUR" ;
int dataBytes = 1000, dataSent ;
socDes = socket(AF_INET, SOCK_STREAM, 0);
if(socDes==-1)
{
perror("cannot create socket") ;
return 0 ;
}
printf("Socket is created\n");
thatAddr.sin_family = AF_INET ;
thatAddr.sin_addr.s_addr = inet_addr(SERVERIP) ; 
thatAddr.sin_port = htons(PORT) ; 
int thatAddrLen = sizeof(thatAddr) ;
printf("%d \n",thatAddr.sin_family);
if(inet_pton(AF_INET, "127.0.0.1", &thatAddr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
printf("hello there \n");
if(connect(socDes,&thatAddr,sizeof(thatAddr))<0) 
		{
		perror("Unable to connect to server\n");
		exit(0);
		}
    else
        printf("Connected to server\n");


char ar[1024];
/*int shmid=shmget(5678,sizeof(int),0777|IPC_CREAT);
printf("Shmid=%d\n",shmid);
ar=(int *)shmat(shmid,0,0);*/



while(1)
{
printf("Hi from 2\n");
char si[100];
printf("Client: Enter the position =");
scanf("%s",si);
send(socDes,si,1024, 0);


}

close(socDes) ;
return 0 ;
}
