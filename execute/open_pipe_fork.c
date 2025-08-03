#include "../libraries/minishell.h"
#include "../libraries/execute.h"

void create_pipes(int pipe_count, int pipes[][2])
{
	int i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe"); //free eklenir.
			exit(1);
		}
		i++;
	}
}

void close_pipes(int pipe_count, int pipes[][2])
{
	int i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void path_found_and_execute(t_parser *current, char **env)
{
	char *path;
	path = find_path(current->args[0], env);

    if (!path)
    {
        ft_putstr_fd("Komut bulunamadi: ", 2);
		ft_putendl_fd(current->args[0], 2);
		exit(127); // düzeltilicek
    }
    execve(path, current->args, env);
    perror("execve"); // düzeltilecek
    exit(127);
}

void wait_all_child_process(t_parser *parser)
{
	int i;

	i = 0;
	while (i < count_cmd(parser))
	{
		wait(NULL);
		i++;
	}
}

void forks_and_exec_commands(t_parser *parser, int pipes[][2], int pipe_count, char **env)
{
	t_parser *current;
	int i;
	pid_t pid;

	current = parser;
	i = 0;
	while (current)
    {
        pid = fork();
        if (pid == 0)
        {
            // redirector(current); // bu fonksiyon yazılacak.
            if (i != 0)
                dup2(pipes[i - 1][0], 0);
            if (current->next)
                dup2(pipes[i][1], 1);
            close_pipes(pipe_count, pipes);
            path_found_and_execute(current, env);
    	}
        current = current->next;
        i++;
	}	
}


void execute_main(t_parser *parser, char **env)
{
    int pipe_count;
    int i;
    pid_t pid;
    t_parser *current;
    
    i = 0;
    current = parser;
    pipe_count = count_cmd(parser) - 1;
    int pipes[pipe_count][2];
	create_pipes(pipe_count, pipes);
	forks_and_exec_commands(parser, pipes, pipe_count, env);
	close_pipes(pipe_count, pipes);
    wait_all_child_process(parser);
}