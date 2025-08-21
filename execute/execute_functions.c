/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:32:17 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 18:14:55 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

extern sig_atomic_t	g_signal;

void	check_signal_and_built(int control_value, t_main_struct *main_struct,
		t_parser *parser, char **cmd)
{
	pid_t	pid;
	int		sig;

	pid = fork();
	this_is_not_built_in_fork(cmd, main_struct, parser, pid);
	waitpid(pid, &control_value, 0);
	if (WIFSIGNALED(control_value))
	{
		sig = WTERMSIG(control_value);
		if (sig == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			main_struct->last_status = 130;
		}
		else if (sig == SIGQUIT)
		{
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			main_struct->last_status = 131;
		}
	}
	else if (WIFEXITED(control_value))
		main_struct->last_status = WEXITSTATUS(control_value);
}

void	main_run_built_in(t_main_struct *main_struct, t_parser *parser)
{
	int	return_code;

	setup_signals(EXECUTING);
	return_code = run_built_in(parser, main_struct);
	if (return_code < 0)
		main_struct->last_status = 1;
	else
		main_struct->last_status = return_code;
	setup_signals(INTERACTIVE);
}

void	this_is_not_built_in(char **cmd, t_main_struct *main_struct,
		t_parser *parser)
{
	char	*path;

	(void)parser;
	if (!cmd || !cmd[0] || cmd[0][0] == '\0')
		return ;
	path = control_path(cmd, main_struct);
	is_acces_path(path);
	exec_or_die(path, cmd, main_struct);
}

void	prepare_execute_for_one_cmd(t_parser *parser,
		t_main_struct *main_struct)
{
	if (!parser->args || !parser->args[0] || parser->args[0][0] == '\0')
	{
		if (prepare_heredocs(parser, *(main_struct->env_struct), main_struct)
			== -1)
			return ;
		setup_signals(EXECUTING);
		if (main_redirector(parser, 0) == -1)
			main_struct->last_status = 1;
		setup_signals(INTERACTIVE);
		return ;
	}
	if (prepare_heredocs(parser, *(main_struct->env_struct), main_struct) == -1)
		return ;
	setup_signals(EXECUTING);
	if (main_redirector(parser, 0) == -1)
	{
		main_struct->last_status = 1;
		return ;
	}
	if (parser->built_type >= 0 && parser->built_type <= 6)
		main_run_built_in(main_struct, parser);
	else
		check_signal_and_built(0, main_struct, parser, parser->args);
}

void	prepare_execute(char **cmd, t_main_struct *main_struct,
		t_parser *parser)
{
	int	cmd_count;
	int	control_value;
	int	control_redirector;

	g_signal = 0;
	control_value = 0;
	cmd_count = count_cmd(parser);
	if (cmd_count == 1)
		prepare_execute_for_one_cmd(parser, main_struct);
	else
	{
		setup_signals(EXECUTING);
		execute_main(parser, main_struct);
	}
	setup_signals(INTERACTIVE);
}
