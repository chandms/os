#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9491
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

	int sockfd,newsockfd;

	sockfd= socket(AF_INET,SOCK_STREAM,0);

	if(sockfd<0)
	{
		perror("Server Socket Creation Problem\n");
		exit(0);
	}

	struct sockaddr_in thisAddr,thatAddr;
	thisAddr.sin_family= AF_INET;
	thisAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	thisAddr.sin_port= htons(PORT);


	int bd=bind(sockfd,(struct sockaddr *)&thisAddr,sizeof(thisAddr));
	if(bd<0)
	{
		perror("bind problem\n");
		exit(0);
	}

	listen(sockfd,3);

    

	while(1)
	{
		int len=sizeof(thatAddr);
		newsockfd= accept(sockfd,(struct sockaddr*)&thatAddr,(socklen_t *)&len);

		if(newsockfd<0)
		{
			perror("acceptance problem\n");
			exit(0);
		}
		char s[100];
		recv(newsockfd,s,100,0);
		printf("Client has given =%s\n",s);
		int size=strlen(s);
		int ans=calculate(s,size);
		sprintf(s,"%d",ans);
		send(newsockfd,s,100,0);

		close(newsockfd);

	}
	return 0;
}