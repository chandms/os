#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9491


int main()
{

	int sockfd;

	sockfd= socket(AF_INET,SOCK_STREAM,0);

	if(sockfd<0)
	{
		perror("Client Socket Creation Problem\n");
		exit(0);
	}

	struct sockaddr_in thatAddr;
	thatAddr.sin_family= AF_INET;
	thatAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	thatAddr.sin_port= htons(PORT);

	int con= connect(sockfd,(struct sockaddr *)&thatAddr,sizeof(thatAddr));

	if(con<0)
	{
		perror("connection problem\n");
		exit(0);
	}
    while(1)
    {
	char t[100];
	printf("Enter arithmetic exp : ");
	gets(t);
	char s[100];
	int m=0;
	for(int y=0;y<strlen(t)+1;y++)
    {
       if(t[y]!=' ')
       {
       	s[m]=t[y];
       	m++;
       }
    }
	send(sockfd,s,100,0);
	recv(sockfd,s,100,0);
	printf("Result is=%s\n",s);
	}
	return 0;

}