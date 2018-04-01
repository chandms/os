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
	return;
}

void f2()
{
	return;
}
int main()
{
int shmid=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);
int shm=shmget(5678,sizeof(int),0777|IPC_CREAT);
char *ar,*br;
char *nz,*bz;

char buf[100], command[5], filename[20], *f;
int k, size, status;
int filehandle;
pid_t pid;
 
pid=fork();
if(pid==0)
{
	ar=(char *)shmat(shmid,0,0);
    nz=(char *)shmat(shm,0,0);
	printf("This the contoller client.......................................\n");
	int socDes;
	struct sockaddr_in thatAddr;
	int dataBytes, thatAddrLen ,filehandle;
	int dataSent;
	socDes = socket(AF_INET, SOCK_STREAM, 0);
	
	if(socDes==-1) {
	perror("cannot create socket") ;
	return 0 ;
	}
	thatAddr.sin_family = AF_INET ;
	thatAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	thatAddr.sin_port = htons(PORT) ;

	if(connect(socDes,&thatAddr,sizeof(thatAddr))<0) 
		{
		perror("Unable to connect to server\n");
		exit(0);
		}

	int g=0;
	while(1)
	{
		int count=0;
		signal(SIGUSR1,f1);
		printf("Enter Choice 1 for port, 2 for cd , 3 for get , 4 for put and 5 for quit\n");
	    int nn;
	    scanf("%d",&nn);
	    char sd[100];
	    sprintf(sd,"%d",nn);
        strcpy(ar,sd);
	    switch(nn)
	    {
	    	case 1:printf("Got port\n");
	    		   strcpy(buf,"port ");
	    		   char port[100];
	               sprintf(port,"%d",PORT);
	               strcat(buf,port);
	               send(socDes, buf, 100, 0);
	               count++;
				   break;
	    	case 2:
	    		   printf("Got cd\n");
	    		   strcpy(buf, "cd ");
				   printf("Enter the path to change the remote directory: ");
				   scanf("%s", buf + 3);
				   send(socDes, buf, 100, 0);
				   break;
		    case 3:
		    	  printf("Got get\n");
		    	  printf("Enter only  the filename to get and the file must be in server's local directory: ");
				  scanf("%s", filename);
				  strcpy(nz,filename);
				  strcpy(buf, "get ");
				  strcat(buf, filename);
				  send(socDes, buf, 100, 0);
				  break;
	    	case 4:	
	    			printf("Got put\n");
	    			printf("Enter only the filename to put to server, file should be in the client's local directory: ");
		          	scanf("%s", filename);
			  		filehandle = open(filename, O_RDONLY);
		          	if(filehandle == -1)
		            {
		              printf("No such file on the local directory of client\n\n");
		              break;
		            }
		          	strcpy(buf, "put ");
					strcat(buf, filename);
					send(socDes, buf, 100, 0);
					struct stat obj;
					stat(filename, &obj);
					size = obj.st_size;
					send(socDes, &size, sizeof(int), 0);
					sendfile(socDes, filehandle, NULL, size);
					break;
			case 5:printf("Got quit\n");
				   strcpy(buf, "quit");
          		   send(socDes, buf, 100, 0);
          		    break;
				   
		    default:kill(getppid(),SIGKILL);
		            g++;
	    			break;
	    }
	    
        if(g==0)
        {
           kill(getppid(),SIGUSR1);
	       pause();
	       printf("Client got the data\n");
        }
        else if(g==1)
        {
        	printf("Query Finished\n");
        	break;
        }

	       
    	
	}
	close(socDes);


}
else if(pid>0)
{
	
     
	
	br=(char *)shmat(shmid,0,0);
	bz=(char *)shmat(shm,0,0);
	while(1)
	{
	 
	
    signal(SIGUSR1,f2);
    pause();
    int socDes;
	struct sockaddr_in thatAddr;
	int dataBytes, thatAddrLen ,size;
	int dataSent;
	socDes = socket(AF_INET, SOCK_STREAM, 0);
	if(socDes==-1) {
	perror("cannot create socket") ;
	return 0 ;
	}
	thatAddr.sin_family = AF_INET ;
	thatAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	thatAddr.sin_port = htons(PORT2) ;

	if(connect(socDes,&thatAddr,sizeof(thatAddr))<0) 
		{
		perror("Unable to connect to server\n");
		exit(0);
		}
    int r=atoi(br);
    
    int g=0;
    printf("data client is connected to data server\n");
    printf("Receiving data...\n");
    switch(r)
    {
    case 1: printf("port command\n");
    		char po[100];
            recv(socDes,po,100,0);
           if(strcmp(po,"200")==0)
           	printf("Success code=220 is recieved and port no is successfully accessed\n");
           else
           {
            if(strcmp(po,"550")==0)
           		printf("error code=550 is received and port no is out of range\n");
           	kill(pid,SIGKILL);
           	exit(0);
       		}
           break;
    case 2:recv(socDes, &status, sizeof(int), 0);
		   if(status==200)
		       printf("Success code= 200 received and Remote directory successfully changed\n");
		   else
		       printf("Error code=501 received and Remote directory failed to change\n");
		   break;
    case 3:recv(socDes, &size, sizeof(int), 0);
		  if(!size)
		    {
		      printf("Error code=550 is received\n");
		      printf("No such file on the remote directory\n\n");
		    break;
		    }
		    printf("Success code=250 is received\n");
		  f = malloc(size);
		  recv(socDes, f, size, 0);
		  while(1)
		    {
		      filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
		      if(filehandle == -1)
			{
			  sprintf(filename + strlen(filename), "%d", 1);//needed only if same directory is used for both server and client
			}
		      else break;
		    }
		  write(filehandle, f, size);
		  close(filehandle);
		  char df[100]="new";
		  strcpy(df, "cat ");
		  strcat(df, bz);
		  printf("df= %s\n",df);
		  system(df);
		  break;
    case 4: printf("");
            char bg[100];
    		recv(socDes, bg, 100, 0);
			if(strcmp(bg,"550")==0)
			{
				printf("Error code=%d is recieved\n",bg);
			    printf("File failed to be stored to remote machine\n");
				
			}
			else
			{
				printf("success code= %s is received\n",bg);
			    printf("File stored successfully\n");
			}
		    break;
    case 5: printf("");
            char t[20]="hi";
    	   recv(socDes, t, 20, 0);
		   if(strcmp(t,"421")==0)
		    {
		      printf("code= 421 is recieved\n");
		      printf("Server closed\nQuitting..\n");
		      close(socDes);
		      kill(pid,SIGKILL);
		      exit(0);
		    }
		    else
		    {
		    	printf("error code=502 is received\n");
		    	printf("Server failed to close connection\n");
		    }
		    break;

    default: g++;
             break;
    
	}
	if(g==0)
	{
		kill(pid,SIGUSR1);
	}
    close(socDes);
	}
	
    


}


return 0;



}
