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
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>



#define bool int
#define true 1
#define false 0

#define p(s)  semop(s,&pop,1)
#define v(s)  semop(s,&vop,1)
#define n 9
#define PORT 50026
#define PORT2 6756
#define BUFFSIZE 1024
void f1()
{
	//printf("hey ya\n");
	return;
}

void f2()
{
	//printf("Cooleo\n");
	return;
}

int main()
{
char filename[100];
int id=shmget(1234,sizeof(int),0777|IPC_CREAT);
int shmid=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);
char *ar,*br;
char *y,*w;
int fd1[2];
 if (pipe(fd1)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }

pid_t pid;
pid=fork();
if(pid==0)
{
	ar=(char *)shmat(shmid,0,0);
	y=(char *)shmat(id,0,0);
	printf("This is the contoller server.......................................\n");
	int socDes ,newsockfd,filehandle;
	struct sockaddr_in thisAddr, thatAddr;
	int dataBytes, thatAddrLen ;
	int dataSent;
	socDes = socket(AF_INET, SOCK_STREAM, 0);
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
    thatAddrLen = sizeof(thatAddr) ;
    socklen_t thatlen = sizeof(struct sockaddr_in);
	
	newsockfd=accept(socDes,(struct sockaddr*)&thatAddr,&thatlen);
	if(newsockfd<0)
	{
		perror("didn't accept\n");
		return 0;
	}
	int c;
	while(1)
	{
	    printf("Controller server is ready to listen..............\n") ;
	    char s[100],command[5];
	    recv(newsockfd,s,1024,0);
	    sscanf(s, "%s", command);
	    printf("Server recieves=%s\n",s);
	    if(strcmp(command, "fin")!=0 && strcmp(command,"put")!=0)
        {
          signal(SIGUSR1,f1);
          strcpy(ar,s);
	      kill(getppid(),SIGUSR1);
	      pause();
	      printf("Data transfer complete\n");
	      if(strcmp(command,"quit")==0)
	      	exit(0);
        }
        else if(strcmp(command,"put")==0)
        {
          signal(SIGUSR1,f1);
          strcpy(ar,s);
          int c = 0, len;
		  char *f;
		  int size;
		  sscanf(s+strlen(command), "%s", filename);
		  recv(newsockfd, &size, sizeof(int), 0);
		  int i = 1;
		  while(1)
		    {
		      filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
		      if(filehandle == -1)
			{
			  sprintf(filename + strlen(filename), "%d", i);
			}
		      else
			break;
		    }
		  f = malloc(size);
		  recv(newsockfd, f, size, 0);
		  c = write(filehandle, f, size);
		  sprintf(y,"%d",c);
		  close(filehandle);
		  kill(getppid(),SIGUSR1);
	      pause();
	      printf("Data transfer complete\n");
        }
	    else
	    {
	    	
	    	printf("query finished\n");
	    	kill(getppid(),SIGKILL);
	    	break;
	    }
	       
    	

	}
	close(socDes);


}
else if(pid>0)
{
	 
	br=(char *)shmat(shmid,0,0);
	w=(char *)shmat(id,0,0);
	while(1)
	{
	
    signal(SIGUSR1,f2);
    pause();
    int socDes ,newsockfd,size;
	struct sockaddr_in thisAddr, thatAddr;
	int dataBytes, thatAddrLen ;
	
	printf("Data sender Server starts..............................................\n");
	socDes = socket(AF_INET, SOCK_STREAM, 0);
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
	printf("Data server is bound\n");
	if (listen(socDes, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
	thatAddrLen = sizeof(thatAddr) ;
	socklen_t thatlen = sizeof(struct sockaddr_in);
	
	newsockfd=accept(socDes,(struct sockaddr*)&thatAddr,&thatlen);
	if(newsockfd<0)
	{
		perror("didn't accept\n");
		return 0;
	}
    char cmd[5];
    sscanf(br, "%s", cmd);
    printf("cmd=%s\n",cmd);
	int c; 
	
	if(!strcmp(cmd, "cd"))
    {
      
      if(chdir(br+3) == 0)
    	c = 200;
  	  else
    	c = 501;
      send(newsockfd, &c, sizeof(int), 0);
      printf("data is sent to CD\n");
	  kill(pid,SIGUSR1);
    }
    else if(!strcmp(cmd, "bye") || !strcmp(cmd, "quit"))
	{
	  printf("FTP server quitting..\n");
	  char i[5]="421";
	  send(newsockfd, i, 5, 0);
	  printf("data is sent to CD\n");
	  kill(pid,SIGUSR1);
	  close(socDes);
	  exit(0);
	}
	else if(!strcmp(cmd,"port"))
	{
		printf("Port no is accessed....\n");
		char gh[100];
		for(int r=5;r<strlen(br)+1;r++)
			gh[r-5]=br[r];
		int po=atoi(gh);
		printf("port=%d\n",po);
		if(po>=1024 && po<=65535)
			sprintf(gh,"%d",200);
		else
			sprintf(gh,"%d",550);
		send(newsockfd, gh, 100, 0);
      	printf("data is sent to CD\n");
	  	kill(pid,SIGUSR1);

	}
	else if(!strcmp(cmd,"get"))
	{
	  int filehandle;
	  sscanf(br, "%s%s", filename, filename);
	  struct stat obj;
	  stat(filename, &obj);
	  //printf("******filename=%s\n",filename);
	  filehandle = open(filename, O_RDONLY);
	  size = obj.st_size;
	  if(filehandle == -1)
	      size = 0;
	  send(newsockfd, &size, sizeof(int), 0);
	  if(size)
	  sendfile(newsockfd, filehandle, NULL, size);
	  printf("data is sent to CD\n");
	  	kill(pid,SIGUSR1);
	}
	else if(!strcmp(cmd,"put"))
	{
	  int no=atoi(w);
	  printf("w= %s\n",w);
	  if(strcmp(w,"0")==0)
	  	send(newsockfd, "550", 100, 0);
	  else
	  	send(newsockfd, "250", 100, 0);
	  printf("data is sent to CD\n");
	  	kill(pid,SIGUSR1);
     }
    
    close(socDes);

	}




}


return 0;



}
