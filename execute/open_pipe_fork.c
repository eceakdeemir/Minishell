/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_pipe_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:32:37 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/23 10:25:09 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

extern sig_atomic_t	g_signal;

void	path_found_and_execute(t_parser *current, t_main_struct *main_struct)
{
	char	*path;
	char	**temporary_execve_env;

	if (!current->args || !current->args[0] || current->args[0][0] == '\0')
		ft_exit(0);
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

void	arrangement_dup(int **pipes, int pipe_count, t_parser *current, int i)
{
	int	j;

	j = 0;
	reset_signals();
	if (current->redirector && !if_no_heredoc(current->redirector))
		dup2(current->redirector->herodoc_fd, STDIN_FILENO);
	else if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < pipe_count)
		dup2(pipes[i][1], STDOUT_FILENO);
	while (j < pipe_count)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
	if (current->redirector && !if_no_heredoc(current->redirector))
		close(current->redirector->herodoc_fd);
}

static void	handle_child_process(t_parser *current, int pipe_count,
		t_main_struct *main_struct, int i)
{
	int	control_redirector;

	if (current->redirector)
	{
		control_redirector = main_redirector(current, 0);
		if (control_redirector == -1)
			ft_exit(1);
	}
	if (!current->args || !current->args[0])
	{
		if (pipe_count > 0)
			ft_exit(0);
		ft_exit(0);
	}
	if (current->built_type >= 0 && current->built_type <= 6)
		ft_exit(run_built_in(current, main_struct));
	else
		path_found_and_execute(current, main_struct);
}

int	forks_and_exec_commands(t_parser *parser, int **pipes, int pipe_count,
		t_main_struct *main_struct)
{
	t_parser	*current;
	pid_t		pid;
	int			i;
	int			prep;

	prep = prep_control(parser, main_struct);
	current = parser;
	i = 0;
	if (prep == 130 || prep == 1)
		return (prep);
	while (current)
	{
		pid = fork();
		if (pid == 0)
		{
			arrangement_dup(pipes, pipe_count, current, i);
			handle_child_process(current, pipe_count, main_struct, i);
		}
		else if (pid < 0)
			return (perror("fork"), 1);
		close_fd_and_child_control(current, pid, main_struct);
		current = current->next;
		i++;
	}
	return (0);
}

void	execute_main(t_parser *parser, t_main_struct *main_struct)
{
	int	pipe_count;
	int	status;
	int	**pipes;
	int	fres;

	if (!parser)
		return ;
	pipe_count = count_cmd(parser) - 1;
	if (pipe_count < 0)
		pipe_count = 0;
	pipes = create_pipes(pipe_count);
	if (pipe_count > 0 && !pipes)
		return ;
	fres = forks_and_exec_commands(parser, pipes, pipe_count, main_struct);
	if (fres == 130)
	{
		close_all_pipes(pipes, pipe_count);
		main_struct->last_status = 130;
		return ;
	}
	close_all_pipes(pipes, pipe_count);
	wait_all_child(0, status, main_struct, parser);
}
