/*Write a C program to implement the following game. The parent program P first creates two pipes, and then spawns
two child processes C and D. One of the two pipes is meant for communications between P and C, and the other for
communications between P and D. Now, a loop runs as follows. In each iteration (also called round), P first randomly
chooses one of the two flags: MIN and MAX (the choice randomly varies from one iteration to another). Each of the
two child processes C and D generates a random positive integer and sends that to P via its pipe. P reads the two
integers; let these be c and d. If P has chosen MIN, then the child who sent the smaller of c and d gets one point. If P
has chosen MAX, then the sender of the larger of c and d gets one point. If c = d, then this round is ignored. The child
process who first obtains ten points wins the game. When the game ends, P sends a user-defined signal to both C and D,
and the child processes exit after handling the signal (in order to know who was the winner). After C and D exit, the
parent process P exits. During each iteration of the game, P should print appropriate messages (like P's choice of the
flag, the integers received from C and */





#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
int p1[2],p2[2];
static int ac=0,ad=0;

void f1(int n)
{
	printf("%d signal caught and pid=%d\n",n,getpid());
	ac=1;
}
void f2(int n)
{
	printf("%d signal caught and pid=%d\n",n,getpid());
	ad=1;
}
void f3(int n)
{
	printf("%d signal caught and pid=%d\n",n,getpid());
	ac=-1;
}
void f4(int n)
{
	printf("%d signal caught and pid=%d\n",n,getpid());
	ad=-1;
}
int main()
{
	
	pid_t pid1,pid2;
	int c=0,d=0;
	int r=1;
	pipe(p1);
	pipe(p2);
	pid1=fork();
		if(pid1!=0)
		{
			pid2=fork();
			if(pid2==0)
			{
				while(1)
				{
					
					signal(SIGALRM,f2);
					pause();
					printf("PID childd=%d\n",getpid());
					if(ad)
					{
					time_t t;
					srand((unsigned)time(&t)%getpid());
					int x=rand()%1000;
					close(p2[0]);
					char s[100];
					sprintf(s,"%d",x);
					write(p2[1],s,100);
					printf("D child writes %d\n",x);
					kill(getppid(),SIGUSR2);
					}

				}
			}
			else 
			{
				time_t t;
				while(1)
				{
					printf("Root pid=%d\n",getpid());
					srand(time(NULL));
					int ch=rand()%2;
					kill(pid1,SIGALRM);
					kill(pid2,SIGALRM);
					signal(SIGUSR1,f3);
					signal(SIGUSR2,f4);
					pause();
					//pause();
					if(ac==-1 && ad==-1)
					{
					if(ch==0)
						printf("MIN chosen\n");
					else
						printf("MAX chosen\n");
					close(p1[1]);
					close(p2[1]);
					char s1[100],s2[100];
					read(p1[0],s1,100);
					read(p2[0],s2,100);
					int x=atoi(s1);
					int y=atoi(s2);
					printf("obtained from c=%d and d=%d\n",x,y);
					if(ch==0)
					{
						if(x<y)
							c++;
						else if(y<x)
							d++;
					}
					else
					{
						if(x>y)
							c++;
						else if(y>x)
							d++;
					}
					printf("round=%d\n",r);
					printf("Score of c =%d and Score of d=%d\n",c,d);
					printf("\n\n\n");
					if(c==10 || d==10)
					{
						printf("round=%d\n",r);
						if(c==10 && d!=10)
							printf("C winner\n");
						else if(d==10 && c!=10)
							printf("D winner\n");
						else
							printf("Draw\n");
						kill(pid1,SIGKILL);
						kill(pid2,SIGKILL);
						exit(0);
					}
					r++;
					ad=0;
					ac=0;
					sleep(1);

				}
				}
			}
		}
		else
		{
			while(1)
			{
				printf("PID childc=%d\n",getpid());
				signal(SIGALRM,f1);
				pause();
				if(ac)
				{
				time_t t;
				srand((unsigned)time(&t)%getpid());
				int x=rand()%1000;
				close(p1[0]);
				char s[100];
				sprintf(s,"%d",x);
				write(p1[1],s,100);
				printf("C Child writes %d\n",x);
				kill(getppid(),SIGUSR1);
				}	
			}
		}
	return 0;
}