
/*In this assignment, you are asked to implement the producer-consumer problem. In the system, you have
one producer process and one consumer process accessing a shared buffer of limited size BMAX. The
producer and the consumer are the child and the parent processes, respectively. Implement the shared
buffer (circular queue) with the help of a shared memory (initially empty, containing –1). While
executing, the producer process produces an item (essentially, the item number, starting from 0) and
inserts it in the buffer. The consumer process, while executing, removes the item from the buffer and
replaces it by –1. You must ensure that the order, in which items are removed by the consumer, must
follow the order of insertion (by the producer). The producer must wait once the buffer is full (number of
items reaches BMAX). After the consumer consumes an item, it wakes up the producer. Likewise, the
consumer process waits when the buffer is empty, and is woken up by the producer after a data item is
inserted.
Print appropriate messages to indicate the production and consumption sequences. Insert appropriate
delays (sleep() or usleep()) so that the messages become visible.*/







#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define p(s)  semop(s,&pop,1)
#define v(s)  semop(s,&vop,1)
#define n 5

int main()
{
	//int in=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);
	//int out=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);
	int in,out;

	int shmid=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);

	int mc=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	int mp=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	int ne=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	int nf=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	in=0;
	out=0;
	int count=6;
    
    semctl(mc,0,SETVAL,1);
    semctl(mp,0,SETVAL,1);
    semctl(ne,0,SETVAL,n);
    semctl(nf,0,SETVAL,0);
    
    struct sembuf pop,vop;
    pop.sem_num=0;
    vop.sem_num=0;
    pop.sem_flg=0;
    vop.sem_flg=0;
    pop.sem_op=-1;
    vop.sem_op=1;

    int *a,*b;
    pid_t pid1;
    pid1=fork();
    if(pid1==0)
    {
    	a=(int *)shmat(shmid,0,0);
    	int data=0;
    	for(int j=0;j<n;j++)
    		a[j]=-1;
    	while(count)
    	{
    		 sleep(1);
    	  
           int m=data;
           p(mp);
           p(ne);
           a[in]=m;
           printf("producer writes %d at %dth place\n",m,in);
           in=(in+1)%n;
           count--;
           data++;
           v(nf);
           v(mp);

           	int k=0;
    	   for(int j=0;j<n;j++)
    	   {
    	   	if(a[j]==-1)
    	   		k++;
    	   }
    	   if(k==0)
    	   {
    	   	printf("Buffer Full:Producer waits\n");
    	   	v(mc);
    	   }


    	}
    	shmdt(a);
    }
    else
    {
    	b=(int *)shmat(shmid,0,0);
    	int data=0;
    	while(count)
    	{
    		sleep(4);
    	   
    		
    		p(mc);
    		p(nf);
    		int y=b[out];
    		b[out]=-1;
    		printf("Consumer reads %d from %dth place\n",y,out);
    		out=(out+1)%n;
    		count--;
    		v(ne);
    		v(mc);
    		int k=0;
    	   for(int j=0;j<n;j++)
    	   {
    	   	if(b[j]!=-1)
    	   		k++;
    	   }
    	   if(k==0)
    	   {
    	   	printf("Buffer Empty:Consumer waits\n");
    	   	v(mp);
    	   }

    	}
    	   shmdt(b);
    }

    return 0;

}