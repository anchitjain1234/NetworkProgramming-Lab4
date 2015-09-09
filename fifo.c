#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>

int main(int argc, char const *argv[])
{
	pid_t pid[4];
	int rfd,wfd;
	if(mkfifo("fif",0777)<0 || mkfifo("fif1",0777)<0 || mkfifo("fif2",0777)<0 )
		perror("Error in creating fifo\n");

	pid[0]=fork();
	if(pid[0] == 0)
	{
		close(1);
		wfd=open("fif",O_WRONLY);
		execlp ("ls", "ls", "-l", (char *) 0);
	}
	else
	{
		pid[1]=fork();
		if(pid[1] == 0)
		{
			close(1);
			close(0);
			rfd=open("fif",O_RDONLY);
			wfd=open("fif1",O_WRONLY);
			wait(pid[0]);
			execlp ("grep", "grep", "^d", (char *) 0);
		}
		else
		{
			pid[2]=fork();
			if(pid[2] == 0)
			{
				close(1);
				close(0);
				rfd=open("fif",O_RDONLY);
				wfd=open("fif2",O_WRONLY);
				wait(pid[0]);
				execlp ("grep", "grep", "^-", (char *) 0);
			}
			else
			{
				pid[3] = fork();
				if(pid[3] == 0)
				{
					close(0);
					rfd=open("fif1",O_RDONLY);
					wait(pid[1]);
					fprintf(stderr, "directories\n");
					execlp ("wc", "wc", "-l", (char *) 0);
				}
				else
				{
					close(0);
					rfd=open("fif2",O_RDONLY);
					wait(pid[2]);
					fprintf(stderr, "files\n");
					execlp ("wc", "wc", "-l", (char *) 0);
				}
			}
		}
	}
	return 0;
}