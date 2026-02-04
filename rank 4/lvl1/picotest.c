#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int picoshell(char **cmds[])
{
    pid_t pid;
    int pipe_fd[2];
    int i = 0;
    int status;
    int prev_fd = -1;

    while(cmds && cmds[i])
    {
        if (cmds[i+1] && pipe(pipe_fd) == -1)
            return (1);
        pid = fork();
        if (pid ==-1)
        {
            if(cmds[i+1])
            {   
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            return(1);
        }
        if  (pid == 0)
        {
            if (prev_fd != -1)
            {
                if(dup2(prev_fd, STDIN_FILENO) == -1)
                    exit(1);
                close(prev_fd);
            }
            if(cmds[i+1])
            {
                close(pipe_fd[0]);
                if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
                    exit(1);
                close (pipe_fd[1]);
            }
            execvp(cmds[i][0],cmds[i]);
            exit(1);
        }
        if(prev_fd != -1)
            close (prev_fd);
        if(cmds[i + 1])
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }
        i++;
    }
    while(wait(&status) > 0)
    {
        if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
            return (1);
    }
    return(0);
}

#include <stdio.h>

int main(void)
{
    char *cmd1[] = {"/bin/ls", NULL};
    char *cmd2[] = {"/usr/bin/grep", "picoshell", NULL};
    char **pipeline1[] = {cmd1, cmd2, NULL};

    char *cmd3[] = {"echo", "squalala", NULL};
    char *cmd4[] = {"cat", NULL};
    char *cmd5[] = {"sed", "s/a/b/g", NULL};
    char **pipeline2[] = {cmd3, cmd4, cmd5, NULL};

    printf("Test 1:\n");
    picoshell(pipeline1);

    printf("\nTest 2:\n");
    picoshell(pipeline2);

    return 0;
}


