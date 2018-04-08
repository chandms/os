#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 8494

int calc(int a,int b,char s)
{
	switch(s){
		case '+':return a+b;
		case '-':return a-b;
		case '*':return a*b;
		case '/':return a/b;
	}
}
int calculate(char s[100],int size)
{
	printf("Hi size=%d and s=%s\n",size,s);
	int i=0;
	char f[100],n[100];
	int e=0,w=0;
	while(i<size && s[i]!='+' && s[i]!='*' && s[i]!='/' && s[i]!='-')
	{
         f[e]=s[i];
         e++;
         i++;
	}
	f[e]='\0';
	
	while(i<size)
	{
		
		
		char op=s[i];
	
		i++;
		w=0;
		while( i<size && s[i]!='+' && s[i]!='*' && s[i]!='/' && s[i]!='-')
		{
             n[w]=s[i];
        
             w++;
             i++;
		}
		n[w]='\0';

		int a=atoi(f);
		int b=atoi(n);
		int c=calc(a,b,op);
		sprintf(f,"%d",c);

	}
	int res=atoi(f);
	return res;
}

int main()
{

int socdes;

socdes= socket(AF_INET,SOCK_DGRAM,0);

if(socdes<0)
{
	perror("Creation prob\n");
	exit(0);
}

struct sockaddr_in thisAddr,thatAddr;
thatAddr.sin_family= AF_INET;
thatAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
thatAddr.sin_port=htons(PORT);

int bd=bind(socdes,(struct sockaddr *)&thatAddr,sizeof(thisAddr));
if(bd<0)
{
	perror("Bind Problem\n");
	exit(0);
}


while(1)
{
   char s[100];
   int len=sizeof(thatAddr);
   recvfrom(socdes,s,100,0,(struct sockaddr *)&thatAddr,(socklen_t *)&len);
   printf("Client enters : %s\n",s);
   int cal=calculate(s,strlen(s));
   sprintf(s,"%d",cal);
   sendto(socdes,s,100,0,(struct sockaddr*)&thatAddr,(socklen_t)len);

}

return 0;
}