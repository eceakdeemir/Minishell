/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:32:32 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/19 18:49:13 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	is_there_signal_at_child(pid_t pid, int status,
		t_main_struct *main_struct, t_wait_ctx *ctx)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			ctx->saw_sigint = 1;
		if (pid == main_struct->last_child_pid)
		{
			main_struct->last_status = 128 + sig;
			ctx->set_last = 1;
		}
	}
	else if (WIFEXITED(status) && pid == main_struct->last_child_pid)
	{
		main_struct->last_status = WEXITSTATUS(status);
		ctx->set_last = 1;
	}
}

int	count_cmd(t_parser *parser)
{
	int	i;

	i = 0;
	while (parser)
	{
		if (parser->args != NULL)
			i++;
		parser = parser->next;
	}
	return (i);
}

void	create_pipes(int pipe_count, int **pipes)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			ft_exit(1);
		}
		i++;
	}	
}

void	close_pipes(int pipe_count, int **pipes)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
