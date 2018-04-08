#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/select.h>


#define PORT 30233
#define PORT2 9394


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
	//printf("Hi size=%d and s=%s\n",size,s);
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

/* tcp server making */

int socdes,newsockfd;
socdes=socket(AF_INET,SOCK_STREAM,0);
if(socdes<0)
{
	perror("tcp server creation problem\n");
    exit(0);
}

struct sockaddr_in thisAddr,thatAddr;

thisAddr.sin_family=AF_INET;
thisAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
thisAddr.sin_port=htons(PORT);

int bd=bind(socdes,(struct sockaddr *)&thisAddr,sizeof(thisAddr));
if(bd<0)
{
	perror("bind problem\n");
	exit(0);
}
listen(socdes,2);
int thatlen= sizeof(thatAddr);
newsockfd=accept(socdes,(struct sockaddr*)&thatAddr,(socklen_t *)&thatlen);

if(newsockfd<0)
{
	perror("acceptance problem\n");
	exit(0);
}

printf("Tcp client is accepted\n");

/////////////////////////////////////////////////

/* udp server making*/


int sockdgram;
sockdgram = socket(AF_INET,SOCK_DGRAM,0);
if(sockdgram<0)
{
	perror("udp socket creation problem\n");
	exit(0);
}

struct sockaddr_in this,that;

this.sin_family=AF_INET;
this.sin_addr.s_addr=inet_addr("127.0.0.1");
this.sin_port=htons(PORT2);

char uc[100];
int tlen=sizeof(that);
int bd2=bind(sockdgram,(struct sockaddr*)& this,sizeof(this));
if(bd2<0)
{
	perror("Bind problem in udp\n");
	exit(0);
}

int n=sockdgram+1;

while(1)
{
	fd_set rset;
	FD_ZERO(&rset);

	FD_SET(socdes,&rset);
	FD_SET(sockdgram,&rset);

	int v=select(n,&rset,NULL,NULL,NULL);

	if(v==0)
	{
		printf("No request\n");
	}
	else if(v<0)
	{
		printf("Error\n");
	}
	else
	{
		if(FD_ISSET(socdes,&rset))
		{
           char s[100];
           recv(newsockfd,s,100,0);
           printf("TCP Client entered =%s\n",s);
           int r=calculate(s,strlen(s));
           printf("Result=%d\n",r);
           printf("------------------------------------\n");
		}
		if(FD_ISSET(sockdgram,&rset))
		{
           char s[100];
           recvfrom(sockdgram,s,100,0,(struct sockaddr *)&that,(socklen_t *)& tlen);
           printf("UDP Client entered =%s\n",s);
           int r=calculate(s,strlen(s));
           printf("Result=%d\n",r);
           printf("**************************************\n");
		}
	}
}


return 0;




}