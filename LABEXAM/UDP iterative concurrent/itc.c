#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 8494

int main()
{

int socdes;

socdes= socket(AF_INET,SOCK_DGRAM,0);

if(socdes<0)
{
	perror("Creation prob\n");
	exit(0);
}

struct sockaddr_in thatAddr;
thatAddr.sin_family= AF_INET;
thatAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
thatAddr.sin_port=htons(PORT);


while(1)
{
   char s[100],t[100];
   int len=sizeof(thatAddr);
   printf("Enter arithmetic exp : ");
   gets(t);
   int m=0;
   for(int j=0;j<strlen(t)+1;j++)
   {
   	   if(t[j]!=' ')
   	   {
       s[m]=t[j];
       m++;
   	   }
   }
   sendto(socdes,s,100,0,(struct sockaddr*)&thatAddr,(socklen_t)len);
   recvfrom(socdes,s,100,0,(struct sockaddr *)&thatAddr,(socklen_t *)&len);
   printf("Result= %s\n",s);
   sleep(1);
   

}

return 0;
}