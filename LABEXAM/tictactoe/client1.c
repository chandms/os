#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define PORT 37827


int main()
{
	int sockDes;
	sockDes= socket(AF_INET,SOCK_STREAM,0);
	if(sockDes<0)
	{
		perror("Socket Creation problem\n");
		exit(0);
	}

	struct sockaddr_in thatAddr;

	thatAddr.sin_family= AF_INET;
	thatAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	thatAddr.sin_port=htons(PORT);

	int con=connect(sockDes,(struct sockaddr *)&thatAddr,sizeof(thatAddr));

	if(con<0)
	{
		printf("Connection problem\n");
		exit(0);
	}

	printf("Connection done\n");

    while(1)
    {
	char s[100];
	recv(sockDes,s,100,0);
	for(int j=0;j<3;j++)
	{
		for(int k=0;k<3;k++)
		{
			int u=j*3+k;
			printf("%c ",s[u]);
		}
		printf("\n");
	}
	printf("Enter the position \n");
	int y;
	scanf("%d",&y);
	s[y-1]='X';
	send(sockDes,s,100,0);
    }
	
	return 0;

}
