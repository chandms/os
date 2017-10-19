/*Write a C program that takes a file name as a command line parameter and sorts a
set of integers stored in the file (use any sorting method). You can assume that the
file will always be there in the current directory and that it will always contain a
set of integers (maximum no. of integers is 1000). The sorted output is written to
the display and the input file is left unchanged. Compile the C file into an
executable named "sort1". Name the C file sort1.c.
Now write a C program (xsort.c) that implements a command called "xsort" that
you will invoke from the shell prompt. The syntax of the command is "xsort
<filename>". When you type the command, the command opens a new xterm
window, and then sorts the integers stored in the file <filename> using the
program "sort1". Look up the man pages for xterm, fork and the different
variations of exec* calls (such as execv, execve, execlp etc.) to do this assignment. */


#include <stdio.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	char *args[]={"./sort",argv[argc-1],NULL};
	printf("In xsort\n");
	execvp(args[0],args);

	printf("will not come\n");
	return 0;

}
