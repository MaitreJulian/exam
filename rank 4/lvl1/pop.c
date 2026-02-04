#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	if(!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	int pipefd[2];
	if (pipe(pipefd) == -1)
		return (-1);
	pid_t pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(pipefd[0]);
			dup2(pipefd[1], 1);
			close(pipefd[1]);
		}
		else
		{
			close(pipefd[1]);
			dup2(pipefd[0], 0);
			close(pipefd[0]);
		}
		execvp(file, argv);
		exit(1);
	}
	if (type == 'r')
	{
		close(pipefd[1]);
		return (pipefd[0]);
	}
	else
	{
		close(pipefd[0]);
		return (pipefd[1]);
	}
}


/* petit helper */
void fatal(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(void)
{
	int fd_ls;
	int fd_grep;
	int fd_wc;
	char buffer[256];
	ssize_t r;

	printf("===== TEST 1: ls | grep c | wc -l =====\n");

	/* ls */
	fd_ls = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
	if (fd_ls == -1)
		fatal("ft_popen ls");

	/* grep c */
	dup2(fd_ls, STDIN_FILENO);
	close(fd_ls);
	fd_grep = ft_popen("grep", (char *const []){"grep", "c", NULL}, 'r');
	if (fd_grep == -1)
		fatal("ft_popen grep");

	/* wc -l */
	dup2(fd_grep, STDIN_FILENO);
	close(fd_grep);
	fd_wc = ft_popen("wc", (char *const []){"wc", "-l", NULL}, 'r');
	if (fd_wc == -1)
		fatal("ft_popen wc");

	while ((r = read(fd_wc, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[r] = '\0';
		printf("%s", buffer);
	}
	close(fd_wc);

	printf("\n===== TEST 2: write to cat, read back =====\n");

	int fd_cat = ft_popen("cat", (char *const []){"cat", NULL}, 'w');
	if (fd_cat == -1)
		fatal("ft_popen cat (w)");

	write(fd_cat, "hello\n", 6);
	write(fd_cat, "this is a test\n", 15);
	write(fd_cat, "popen is fun\n", 13);
	close(fd_cat);

	printf("OK (no deadlock)\n");

	printf("\n===== TEST 3: big output (yes | head) =====\n");

	int fd_yes = ft_popen("yes", (char *const []){"yes", "42", NULL}, 'r');
	if (fd_yes == -1)
		fatal("ft_popen yes");

	dup2(fd_yes, STDIN_FILENO);
	close(fd_yes);

	int fd_head = ft_popen("head", (char *const []){"head", "-n", "5", NULL}, 'r');
	if (fd_head == -1)
		fatal("ft_popen head");

	while ((r = read(fd_head, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[r] = '\0';
		printf("%s", buffer);
	}
	close(fd_head);

	printf("\n===== TEST 4: error handling =====\n");

	int fd_err = ft_popen("does_not_exist",
		(char *const []){"does_not_exist", NULL}, 'r');
	if (fd_err == -1)
		printf("Correctly failed\n");
	else
	{
		printf("Unexpected success\n");
		close(fd_err);
	}

	printf("\n===== DONE =====\n");
	return 0;
}
