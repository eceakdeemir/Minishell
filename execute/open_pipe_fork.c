/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_pipe_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:32:37 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/17 19:45:58 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

extern sig_atomic_t	g_signal;

void	path_found_and_execute(t_parser *current, t_main_struct *main_struct)
{
	char	*path;
	char	**temporary_execve_env;

	path = find_path(current->args[0], main_struct);
	if (!path)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putendl_fd(current->args[0], 2);
		ft_exit(127);
	}
	env_converter_to_execve(main_struct, &temporary_execve_env);
	execve(path, current->args, temporary_execve_env);
	perror("execve");
	ft_exit(126);
}

static int if_no_heredoc(t_parser *parser)
{
	t_parser *current;

	current = parser;
	while (current && current->redirector)
	{
		if (current->redirector->token_enum == TOKEN_HEREDOC)
			return(0);
		current = current->next;
	}
	return(1);
} 
void	arrangement_dup(int **pipes, int pipe_count, t_parser *current,
		int i)
{
	reset_signals();
	if (i != 0 && if_no_heredoc(current))
		dup2(pipes[i - 1][0], 0);
	if (if_no_heredoc(current) && current->next)
		dup2(pipes[i][1], 1);
	close_pipes(pipe_count, pipes);
}

static int heredoc_count(t_parser *parse_node)
{
	int	i;
	t_redirector *tmp;

	i = 0;
	tmp = parse_node->redirector;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

static int	heredoc_for_pipe(char *limiter, t_parser *parser,
		t_main_struct *main_struct, int hd_no_quoted)
{
	char	*line;
	int i;
	int **pipefd;
	t_redirector *tmp;
	
	pipefd = mem_malloc(sizeof(int *) * heredoc_count(parser));
	i = 0;
	while (i < heredoc_count(parser))
	{
		pipefd[i] = mem_malloc(sizeof(int) * 2);
		if (pipe(pipefd[i]) == -1)
		{
			return(1);
		}
		i++;
	}

	tmp = parser->redirector;
	i = 0;
	while (tmp)
	{
		if (tmp->token_enum != TOKEN_HEREDOC)
		{
			tmp = tmp->next;
			continue;
		}
		while (1)
		{
			line = mem_absorb(readline("> "));
			if (!line)
				break ;
			if (!hd_no_quoted)
				line = heredoc_tokenize_expender(line, *(main_struct->env_struct), main_struct);
			if (line && ft_strcmp(line, tmp->file) == 0)
				break ;
			ft_putendl_fd(line,pipefd[i][1]);
		}
		close(pipefd[i][1]);
		i++;
		tmp = tmp->next;
	}
	return(pipefd[i - 1][0]);
}

int	forks_and_exec_commands(t_parser *parser, int **pipes, int pipe_count,
		t_main_struct *main_struct)
{
	t_parser	*current;
	int			i;
	pid_t		pid;
	int			control_value;
	int			control_redirector;

	current = parser;
	i = 0;
	while (current)
	{
		pid = fork();
		if (pid == 0)
		{
			arrangement_dup(pipes, pipe_count, current, i);
			if (current && current->redirector)
			{
				if (!if_no_heredoc(current))
				{
					current->redirector->herodoc_fd = heredoc_for_pipe(current->redirector->file, current, main_struct, current->redirector->hd_no_expand);
					if (current->next)
						dup2(pipes[i][1], 1);
				}
				control_redirector = main_redirector(current, control_value);
				if (control_redirector == -1)
				{
					main_struct->last_status = 1;
					ft_exit(1);
				}
			}
			if (current->built_type >= 0 && current->built_type <= 6)
				ft_exit(run_built_in(current, main_struct));
			else
				path_found_and_execute(current, main_struct);
		}
		else if (current->next == NULL)
			main_struct->last_child_pid = pid;
		current = current->next;
		i++;
	}
	return (0);
}

void	wait_all_child(int i, int status, t_main_struct *main_struct,
		t_parser *parser)
{
	pid_t		pid;
	t_wait_ctx	ctx;

	ctx.saw_sigint = 0;
	ctx.set_last = 0;
	i = count_cmd(parser);
	while (i > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid > 0)
		{
			i--;
			is_there_signal_at_child(pid, status, main_struct, &ctx);
		}
	}
	if (ctx.saw_sigint)
		write(STDOUT_FILENO, "\n", 1);
	if (!ctx.set_last && ctx.saw_sigint)
		main_struct->last_status = 130;
	g_signal = 0;
}

void	execute_main(t_parser *parser, t_main_struct *main_struct)
{
	int			pipe_count;
	int			status;
	int			i;
	t_parser	*current;
	int			**pipes;


	i = -1;
	pipe_count = count_cmd(parser) - 1;
	pipes = mem_malloc(sizeof(int *) * pipe_count);
	while (++i < pipe_count)
		pipes[i] = mem_malloc(sizeof(int) * 2);
	create_pipes(pipe_count, pipes);
	forks_and_exec_commands(parser, pipes, pipe_count, main_struct);
	close_pipes(pipe_count, pipes);
	wait_all_child(i, status, main_struct, parser);
}
