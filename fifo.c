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
	pid_t pid[6];
	int rfd,wfd,status;
	if(mkfifo("fifi1",0777)<0 || mkfifo("fif2",0777)<0 || mkfifo("fif1",0777)<0 || mkfifo("fifi2",0777)<0)
	{
		perror("Error in creating fifo.Exiting\n");
		exit(0);
	}
	// mkfifo("fifi1",0777);
	// mkfifo("fif2",0777);
	// mkfifo("fif1",0777);
	// mkfifo("fifi2",0777);
	pid[0]=fork();
	if(pid[0] == 0)
	{
		close(1);
		wfd=open("fifi1",O_WRONLY);
		execlp ("ls", "ls", "-l", (char *) 0);
	}
	else
	{
		pid[1] =fork();
		if(pid[1] == 0)
		{
			wait(NULL);
			close(1);
			wfd=open("fifi2",O_WRONLY);
			execlp ("ls", "ls", "-l", (char *) 0);
		}
		else
		{
			pid[2] =fork();
			if(pid[2] == 0)
			{
				wait(NULL);
				close(1);
				close(0);
				rfd=open("fifi1",O_RDONLY);
				wfd=open("fif1",O_WRONLY);
				execlp ("grep", "grep", "^d", (char *) 0);
			}
			else
			{
				pid[3] =fork();
				if(pid[3] == 0)
				{
					wait(NULL);
					close(1);
					close(0);
					rfd=open("fifi2",O_RDONLY);
					wfd=open("fif2",O_WRONLY);
					execlp ("grep", "grep", "^-", (char *) 0);
				}
				else
				{
					pid[4]=fork();
					if(pid[4] == 0)
					{
						wait(NULL);
						close(0);
						rfd=open("fif1",O_RDONLY);
						// printf("directories\n");
						execlp ("wc", "wc", "-l", (char *) 0);
					}
					else
					{
						wait(NULL);
						pid[5]=fork();
						if(pid[5]==0)
						{
							close(0);
							rfd=open("fif2",O_RDONLY);
							// printf("files\n");
							execlp ("wc", "wc", "-l", (char *) 0);
						}
						else
						{
							wait(NULL);
						}
					}
				}
			}
		}
	}
	wait(NULL);
	remove("fif1");
	remove("fif2");
	remove("fifi1");
	remove("fifi2");
	return 0;
}