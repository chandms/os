/*Write a C program parmax.c that creates a tree of processes in order to recursively compute the maximum in an array of
integers. The process at the root of the tree reads the count n of integers in the array as a command-line parameter. An
array A of size n is then populated with randomly generated integers of small values (in the range 0–127). The initially
unsorted array is printed by the root process.
Any process in the tree handles a chunk of the array A. The chunk is delimited by two indices L and R. For the root
process, L = 0 and R = n – 1. Any process P in the tree (including the root) first counts the number of integers in the
chunk it has got. If that count is less than 10, the process P itself computes the maximum element in its chunk, prints it,
and exits. If the chunk size of P is 10 or more, then P creates two child processes PL and PR which handle the chunks
[L, M] and [M + 1, R] in A respectively, where M = (L + R) / 2. P waits until the two child processes PL and PR exit. It
then computes the maximum of the two maximum values computed by PL and PR, prints this maximum, and exits.
Every non-root process returns to its parent (via the exit status) the maximum value for its chunk. During the printing of
the maximum computed by a process P, the PID and the parent PID of P are also printed.
*/







#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>

int max(int l,int r,int a[100])
{
	int c=r-l;
	if(c<10)
	{
		int ma=INT_MIN;
		for(int j=l;j<=r;j++)
		{
			if(a[j]>ma)
				ma=a[j];
		}
		return ma;
	}
	else
	{
		pid_t pid1,pid2;
		int x=(l+r)/2;
		pid1=fork();
		if(pid1==0)
		{
			int l1=max(l,x,a);
			printf("pid=%d,max=%d\n",getpid(),l1);
			exit(l1);
		}
		else if(pid1>0)
		{
			pid_t pid2;
			pid2=fork();
			if(pid2==0)
			{
			int l2=max(x+1,r,a);
			printf("pid=%d,max=%d\n",getpid(),l2);
			exit(l2);
			}
			else if(pid2>0)
			{
				int r1,r2;
				waitpid(pid1,&r1,0);
				waitpid(pid2,&r2,0);
                r1=r1>>8;
                r2=r2>>8;
                if(r1>r2)
                	return r1;
                else
                	return r2;
			}
		}

	}

}
int main(int argc,char *argv[])
{
     int n=atoi(argv[1]);
     time_t t;
     srand(time(NULL));
     int a[100];
     for(int j=0;j<n;j++)
     {
     	int x=rand()%127;
     	a[j]=x;
     }
     printf("n=%d\n",n);
     for(int j=0;j<n;j++)
     	printf("%d ",a[j]);
     printf("\n");
     int l=0,r=n-1;
     int x=max(l,r,a);
     printf("root-pid =%d and Maximum is %d\n",getpid(),x);
     return 0;
}