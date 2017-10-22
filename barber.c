
//Sleeping Barber's Problem

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>


#define p(s)  semop(s,&pop,1)
#define v(s)  semop(s,&vop,1)
int main()
{
	int count=0;
	int noc=3;
    int ne,nf,cust,barb;
    cust=semget(IPC_PRIVATE,1,0777|IPC_CREAT);
    barb=semget(IPC_PRIVATE,1,0777|IPC_CREAT);

    count=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
    int *a,*b;

    semctl(cust,0,SETVAL,1);
    semctl(barb,0,SETVAL,0);

    struct sembuf pop,vop;

    pop.sem_num=0;
    vop.sem_num=0;
    pop.sem_flg=0;
    vop.sem_flg=0;
    pop.sem_op=-1;
    vop.sem_op=1;

    pid_t pid;
    pid=fork();
    if(pid==0)
    {
    	//customer
    	int m=10;
    	a=shmat(count,0,0);
    	while(m)
    	{
    		sleep(1);
    		printf("\t\t\tCount in cust initially %d\n",a[0]);
	    	p(cust);
	    	a[0]++;
	    	if(a[0]==noc+1)
	    	{
	    		printf("\t\t\tFull waiting room\n");
	    		a[0]--;
	    		v(cust);
	    	}
	    	else if(a[0]==1)
	    	{
	    		printf("\t\t\tFirst customer ,so barber is waken up\n");
	    		v(barb);
	    		v(cust);
	    	}
	    	else if(a[0]>=1)
	    	{
	    		printf("\t\t\tMore than one customer, so have to wait\n");
	    		v(cust);
	    		v(barb);
	    	}
	    	printf("\t\t\tCount in cust finally %d\n",a[0]);
	    	m--;
	    }
    }
    else if(pid>0)
    {
    	//barber
    	int t=noc;
    	b=shmat(count,0,0);
    	while(1)
    	{
    	sleep(2);
        printf("Count in barber initially %d\n",b[0]);
    	p(barb);
    	printf("haircutting .....\n");
    	b[0]--;
    	printf("Count in barber finally %d\n",b[0]);
    	v(cust);
    	}

    }
    return 0;

}