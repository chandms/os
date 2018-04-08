#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 30233

int main()
{
	int socdes;
	socdes= socket(AF_INET,SOCK_STREAM,0);
	if(socdes<0)
	{
		perror("Creation problem\n");
		exit(0);
	}

	struct sockaddr_in thatAddr;

	thatAddr.sin_family=AF_INET;
	thatAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	thatAddr.sin_port=htons(PORT);

	int con= connect(socdes,(struct sockaddr *)&thatAddr,sizeof(thatAddr));

	if(con<0)
	{
		perror("Connection error\n");
		exit(0);
	}
	int i=0;
	while(1)
	{
    char f[100];
    printf("Enter the arithmetic exp : ");
    gets(f);
    char s[100];
    int m=0;
    for(int j=0;j<strlen(f)+1;j++)
    {
    	if(f[j]!=' ')
    	{
    		s[m]=f[j];
    		m++;
    	}
    }
	send(socdes,s,100,0);
	printf("message sent from tcp\n");
	sleep(1);
	}
	return 0;
}