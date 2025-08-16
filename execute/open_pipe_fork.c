/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_pipe_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:32:37 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 18:25:11 by ecakdemi         ###   ########.fr       */
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

void	arrangement_dup(int **pipes, int pipe_count, t_parser *current,
		int i)
{
	reset_signals();
	if (i != 0)
		dup2(pipes[i - 1][0], 0);
	if (current->next)
		dup2(pipes[i][1], 1);
	close_pipes(pipe_count, pipes);
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
