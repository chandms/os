/*Write a C program that will search an array of integers for another given integer.
However, to speedup the search, the search is done in parallel by two child
processes. The parent process reads in the number of integers (max. 100), and
then the integers in an array. It also reads in the integer to be searched. It then
creates two child processes. The first child process searches the first half of the
array, and the second child process searches the second half. If the integer is
found, its index in the array is sent to the parent through a pipe. If it is not found,
a -1 is sent to the parent through a pipe. The parent waits for both child processes
to finish and then prints an appropriate message.
Name your program search.c. */





#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int search(int s,int e,int a[100],int t)
{
	for(int j=s;j<e;j++)
	{
        if(a[j]==t)
        {
        	printf(" got %d\n",j);
        	return j;
        }
	}
	return -1;
}
int main(int argc,char* argv[])
{
     int n;
     n=atoi(argv[1]);
     printf("%d\n",n);
     int a[100];
     for(int j=0;j<n;j++)
     {
     	int x;
     	scanf("%d",&x);
     	a[j]=x;
     }
     printf("Array : ");
     for(int j=0;j<n;j++)
     	printf("%d ",a[j]);
     printf("\n");
     int t;
     scanf("%d",&t);
     printf("No to be searched %d \n",t);
     int p1[2],p2[2];
     int l=0,r=n,m=n/2;
     pid_t pid1,pid2;
     if(pipe(p1)==0 && pipe(p2)==0)
     {
     pid1=fork();
     if(pid1==0)
     {
         int x=search(l,m,a,t);
         char s[100];
         sprintf(s,"%d",x);
         close(p1[0]);
         write(p1[1],s,100);
         exit(0);
     }
     else if(pid1>0)
     {
     	pid2=fork();
     	if(pid2==0)
     	{
     		int x=search(m,r,a,t);
     		  char s[100];
		     sprintf(s,"%d",x);
		     close(p2[0]);
		     write(p2[1],s,100);
		     exit(1);
     	}
     	else if(pid2>0)
     	{
     		int st1,st2;
            waitpid(pid1,&st1,0);
            waitpid(pid2,&st2,0);
            close(p1[1]);
            close(p2[1]);
            char s1[100],s2[100];
            read(p1[0],s1,100);
            read(p2[0],s2,100);
            int f1=atoi(s1);
            int f2=atoi(s2);
            if(f1==-1 && f2==-1)
            	printf("not in array\n");
            else if(f1==-1)
            	printf("from 2nd child and %d\n",f2);
            else if(f2==-1)
            	printf("from 1st child and %d\n",f1);
            else
            {
            	printf("from 1st =%d and from 2nd =%d\n",f1,f2);
            }
     	}
     }
	 }
	 else
	 	printf("error\n");
	 return 0;
}