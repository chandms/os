#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>

#define p(s)  semop(s,&pop,1);
#define v(s) semop(s,&vop,1);
int main()
{
	FILE *fp;
	int count=10;
	int var=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);
	int rc=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);
	int g=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);
	int *a,*b,*c,*d,*e,*l,*m;

	int w=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	int r=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	int r2=semget(IPC_PRIVATE,1,0666|IPC_CREAT);

	semctl(w,0,SETVAL,1);
	semctl(r,0,SETVAL,1);
	semctl(r2,0,SETVAL,1);


    struct sembuf pop,vop;

    pop.sem_num=0;
    vop.sem_num=0;
    pop.sem_flg=0;
    vop.sem_flg=0;
    pop.sem_op=-1;
    vop.sem_op=1;
    int readcount=0;

	pid_t pid1,pid2;
	pid1=fork();
	if(pid1==0)
	{
		a=shmat(var,0,0);
		c=shmat(rc,0,0);
		m=shmat(g,0,0);
		//m[0]=0;
		c[0]=0;
		while(count)
		{
			sleep(1);
			p(r);
			c[0]++;
			printf("initially no of Readers is %d\n",c[0]);
			if(c[0]==1)
			{
				printf("First reader enters\n");
				p(w);
			}
			v(r);
			int data;
			data=a[0];
			printf("Reader reads %d and pid=%d\n",data,getpid());
			p(r);
			c[0]--;
			printf("finally no of Readers is %d\n",c[0]);
			if(c[0]==0)
			{
				printf("Last reader leaves\n\n");
				v(w);
			}
			v(r);
			count--;
		}	


	}
	else if(pid1>0)
	{
		pid2=fork();
		if(pid2==0)
		{
		e=shmat(var,0,0);
		d=shmat(rc,0,0);
		l=shmat(g,0,0);
		//l[0]=0;
		d[0]=0;
		while(count)
		{
			sleep(1);
			p(r2);
			d[0]++;
			printf("**  initially no of Readers is %d\n",d[0]);
			if(d[0]==1)
			{
				printf("** first reader enters\n");
				p(w);
			}
			v(r2);
			int data;
			data=e[0];
			printf("** Reader reads %d and pid=%d\n",data,getpid());
			p(r2);
			d[0]--;
			printf("** finally no of Readers is %d\n",d[0]);
			if(d[0]==0)
			{
				printf("** Last reader leaves\n\n");
				v(w);
			}
			v(r2);
			count--;
		}	
		}
		else
		{
			b=shmat(var,0,0);
			b[0]=0;
			while(count)
			{
				sleep(1);
				p(w);
				

				printf("+++++++++ writer enters\n");
				int data;
				data=b[0];
				printf("+++++++++ Writer reads %d\n",data);
				
				b[0]++;
				
				printf("+++++++++ Writer writes %d and pid=%d\n",b[0],getpid());
				printf("+++++++++ writer leaves\n\n");
				v(w);
				
				count--;

			}
		}
		}
	return 0;
}