#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9394

int main()
{
	int socdes;
	socdes= socket(AF_INET,SOCK_DGRAM,0);
	if(socdes<0)
	{
		perror("Creation problem\n");
		exit(0);
	}

	struct sockaddr_in thatAddr;

	thatAddr.sin_family=AF_INET;
	thatAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	thatAddr.sin_port=htons(PORT);
    int i=0;
	while(1)
	{
    char f[100],s[100];
    printf("Enter the arithmetic exp : ");
    gets(f);
    int m=0;
    for(int j=0;j<strlen(f)+1;j++)
    {
    	if(f[j]!=' ')
    	{
    		s[m]=f[j];
    		m++;
    	}
    }
    int thatlen= sizeof(thatAddr);
	sendto(socdes,s,100,0,(struct sockaddr *)&thatAddr,(socklen_t)thatlen);
	printf("message sent from udp\n");
	sleep(1);

	}

	printf("message sent from udp client\n");
	return 0;
}