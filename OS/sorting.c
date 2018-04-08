#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
	FILE *fp;
	fp=fopen(argv[argc-1],"r");

	int n;
	fscanf(fp,"%d",&n);
	printf("n=%d\n",n);
	int a[100];

	for(int j=0;j<n;j++)
	{
		fscanf(fp,"%d",&a[j]);
	}
	printf("array : ");
	for(int j=0;j<n;j++)
		printf("%d ",a[j]);
	printf("\n");
	for(int j=0;j<n;j++)
	{
		for(int k=0;k<n-j-1;k++)
		{
			if(a[k]>a[k+1])
			{
				int temp=a[k];
				a[k]=a[k+1];
				a[k+1]=temp;
			}
		}
	}

	printf("Sorted result : ");
	for(int j=0;j<n;j++)
	{
		printf("%d ",a[j]);
	}
	return 0;
}
