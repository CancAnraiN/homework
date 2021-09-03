#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/wait.h> //wait()
#include <sys/types.h> //fork()
#include <sys/stat.h>
#define FALSE 0
#define TRUE 1
#define STD_INPUT 0
#define STD_OUTPUT 1

#ifndef _SHELL
	#define _SHELL
	enum
	{
		i_size = 100,
		command_size = 10,
		run_back_pid = 1 
	};
	typedef int bool ;
	void reset_all() ;
	void run() ;
	void read_command() ;
	void create_pipe() ;

#endif

static char i_command[i_size];
static char *process1[command_size], *process2[command_size] ;
static char *o_path = NULL, *i_path = NULL ;
static int status = 0 ;
static bool need_pipe = FALSE ;
static bool shell_exit = FALSE, o_redirect = FALSE, i_redirect = FALSE, run_back = FALSE ;

int main()
{
	run();
	return 0;
}


void run()
{
	while(TRUE)
	{
		printf(" $ ");
		reset_all();
		read_command();
		if(shell_exit)
		{
			printf("Stop\n"); //end shell
			break;
		}
		if(!strcmp(process1[0], "cd"))
		{
			chdir(process1[1]); //function for cd
			continue;
		}
		int pid = fork();//creat a child process(fork()!=-1)
		
		if(pid == -1) // return to parent process 
		{
			perror("fork() error");	//print out the error message
		}
		else if(pid != 0)
		{
			if(!run_back)
			{
				waitpid(pid, &status, 0);//pause proccess until child process over
			}
		}
		else
		{
			if(need_pipe)
			{
				printf("creat a pipe\n");
				create_pipe();
			}
			else
			{
				if(o_redirect && i_redirect)
				{
					
					int output_fd = open(o_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);//Write Only (fcntl.h)
					int input_fd = open(i_path, O_RDONLY); //Read Only
					
					dup2(output_fd, STD_OUTPUT);
					dup2(input_fd, STD_INPUT);
					
					execvp(process1[0], process1);//find command in PATH
					
					close(output_fd);
					close(input_fd);
				}
				else if(o_redirect)
				{
					
					int output_fd = open(o_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
					dup2(output_fd, STD_OUTPUT);
					execvp(process1[0], process1);//find command in PATH
					close(output_fd);
				}
				else if(i_redirect)
				{
					int input_fd = open(i_path, O_RDONLY);
					dup2(input_fd, STD_INPUT);
					execvp(process1[0], process1);//find command in PATH
					close(input_fd);
				}
				else
				{
					execvp(process1[0], process1);//find command in PATH
				}
			}
		}
	}
}

void reset_all()
{
	need_pipe = shell_exit = o_redirect = i_redirect = run_back = FALSE;
	o_path = i_path = NULL;
	memset(i_command, 0, sizeof(i_command)); //clean input_command 
	memset(process1, 0, sizeof(process1));	//clean process1
	memset(process2, 0, sizeof(process2));	//clean process2
}

void read_command()
{
	int pipe_index = 0;
	int split_index = 0;
	int has_filepath = 0;
	int i = 0;
	char *store_command[command_size], *split_str;
	
	fgets(i_command, sizeof(i_command), stdin);
	if(i_command[strlen(i_command) - 1] == '\n')
	{
		i_command[strlen(i_command) - 1] = '\0';
	}
	
	if(!strcmp(i_command, "exit") || !strcmp(i_command, "EXIT")) // exit
	{
		shell_exit = TRUE;
		return;
	}
	
	split_str = strtok(i_command, " ");//split string
	while(split_str != NULL)
	{
		store_command[split_index] = split_str;
		split_index++;
		split_str = strtok(NULL, " ");
	}
	
	for(i = 0 ; i < split_index ; i++)
	{
		if(*store_command[i] == '|')
		{
			need_pipe = TRUE;
			pipe_index = i;
			has_filepath = 0;
			continue;
		}
		
		if(*store_command[i] == '<') // input
		{
			i_redirect = TRUE;
			i_path = store_command[i+1];
			has_filepath++;
			i++;
			continue;
		}
		
		if(*store_command[i] == '>') // output
		{
			o_redirect = TRUE;
			o_path = store_command[i+1];
			has_filepath++;
			i++;
			continue;	
		}
		
		if(*store_command[i] == '&') // run back
		{
			run_back = TRUE;
			break;
		}
		
		if(need_pipe)
		{
			process2[i - pipe_index - 1 - has_filepath] = store_command[i];		
		}
		else
		{
			process1[i - has_filepath] = store_command[i];
		}
	}
	
	printf("output_path : %s\n", o_path);
	printf("input_path : %s\n", i_path);
}

void create_pipe()
{
	int fd[2];
	pipe(fd);
	
	int pid = fork();
	
	if(pid == -1)
	{
		perror("pipe fork error");
	}
	else if(pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STD_OUTPUT);
		close(fd[1]);
		
		if(i_redirect)
		{
			int input_fd = open(i_path, O_RDONLY);
			dup2(input_fd, STD_INPUT);
			execvp(process1[0], process1);//find command in PATH
			close(input_fd);
		}
		else
		{
			execvp(process1[0], process1);//find command in PATH
		}
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STD_INPUT);
		close(fd[0]);
		
		if(o_redirect)
		{
			int output_fd = open(o_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			dup2(output_fd, STD_OUTPUT);
			execvp(process2[0], process2);//find command in PATH
			close(output_fd);
		}
		else
		{
			execvp(process2[0], process2);//find command in PATH
		}
	}
}


