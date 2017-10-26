#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

void f1(int n)
{
	printf("*got %d\n",n);
}
void f2(int n)
{
	printf("-got %d\n",n);
}
void f3(int n)
{
	printf("+got %d\n",n);
}
void f4(int n)
{
	printf("&got %d\n",n);
}
int p1[2],p2[2],w1[2],w2[2];
int main()
{
	 srand(time(NULL));
	pid_t pid1,pid2;
    if(pipe(p1)==0 && pipe(p2)==0 && pipe(w1)==0 && pipe(w2)==0)
    {
    	pid_t pid1,pid2;
    	pid1=fork();
    	
    	if(pid1!=0)
    	{
    		pid2=fork();
    		if(pid2==0)
    		{
    		while(1)
    		{
    			//printf("not working");
    			signal(SIGALRM,f2);
    			pause();
    			//printf("PID childd=%d\n",getpid());
    			char y[100];
                close(w2[1]);
    			read(w2[0],y,100);
    			int ac=atoi(y);
    			if(ac==1)
    			{
    				//printf("Yes\n");
                    time_t t;
                    srand((unsigned)time(&t)%getpid());
                    int c=rand()%1000;
                    char s[100];
                    sprintf(s,"%d",c);
                    close(p2[0]);
                    write(p2[1],s,100);
	                //char y[100];
	                sprintf(s,"%d",0);
	                close(w2[0]);
	    			write(w2[1],s,100);
	    			//printf("done\n");
                    kill(getppid(),SIGUSR2);
    			}

    		}
    		}
    		else if(pid2>0)
    		{
    			int cp=0,dp=0,r=1;
    			while(1)
    			{
    				time_t t;
					//srand((unsigned)time(&t)%getpid());
					int ch=rand()%2;
					printf("ch=%d\n",ch);
    				char q1[100],q2[100];
                    close(w1[0]);
                    write(w1[1],"1",100);
    				kill(pid1,SIGALRM);
    				
    				signal(SIGUSR1,f3);
    				pause();
    				//printf("hfehf");
    				close(w1[1]);
	    			read(w1[0],q1,100);
	    			int acc=atoi(q1);
                    //printf("acc %d \n",acc);
    				close(w2[0]);
                    write(w2[1],"1",100);
                   // printf("Childd done\n");
    				kill(pid2,SIGALRM);
    				signal(SIGUSR2,f4);
    				pause();
    				close(w2[1]);
	    			read(w2[0],q2,100);
	    			int add=atoi(q2);
    				printf("PID parent=%d\n",getpid());
    				
    				
    				
	    			 
	    			printf("%d,%d\n",acc,add);
    				if(acc==0 && add==0)
    				{
    					
    					char s1[100],s2[100];
    					close(p1[1]);
    					read(p1[0],s1,100);
    					close(p2[1]);
    					read(p2[0],s2,100);
    					int x=atoi(s1);
    					int y=atoi(s2);
    					printf("obtained %d and %d \n",x,y);
    					printf("Chosen %d\n",ch);
    					if(ch==0)
    					{
    						printf("Min\n");
    						if(x<y)
    							cp++;
    						else if(y<x)
    							dp++;

    					}
    					else 
    					{
    						printf("Max\n");
    						if(x>y)
    							cp++;
    						else if(y>x)
    							dp++;
    					}
    					printf("%d round and c=%d and d=%d\n\n\n",r,cp,dp);
    					if(cp==10 || dp==10)
    					{
    						if(cp==10)
    							printf("c winner\n");
    						else if(dp==10)
    							printf("d winner\n");
    						printf("%d round and c=%d and d=%d\n",r,cp,dp);
    						kill(pid1,SIGKILL);
    						kill(pid2,SIGKILL);
    						kill(getpid(),SIGKILL);
    					}
    					sleep(1);
    					r++;
    				}
    			}
    		}
    	}
    	else
    	{
    		while(1)
    		{
    			signal(SIGALRM,f1);
    			pause();
    			//printf("PID childc=%d\n",getpid());
    			char y[100];
                close(w1[1]);
    			read(w1[0],y,100);
    			int ad=atoi(y);
    			if(ad==1)
    			{
                    time_t t;
                    srand((unsigned)time(&t)%getpid());
                    int c=rand()%1000;
                    char s[100];
                    sprintf(s,"%d",c);
                    close(p1[0]);
                    write(p1[1],s,100);
                    //char y[100];
                    sprintf(s,"%d",0);
                    //printf("sd %s \n",s);
	                close(w1[0]);
	    			write(w1[1],s,100);
                    kill(getppid(),SIGUSR1);
    			}

    		}
    	}
    }
    return 0;
}