/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_wait.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:48:02 by igurses           #+#    #+#             */
/*   Updated: 2025/08/20 18:50:17 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	finalize_wait_status(t_wait_ctx *ctx, t_main_struct *main_struct)
{
	int	saw_sigint_newline;

	saw_sigint_newline = 0;
	if (ctx->saw_sigint)
	{
		write(STDOUT_FILENO, "\n", 1);
		saw_sigint_newline = 1;
	}
	if (ctx->saw_sigint && !ctx->set_last)
		main_struct->last_status = 130;
	g_signal = 0;
	(void)saw_sigint_newline;
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
		handle_wait_result(pid, &i);
	}
	finalize_wait_status(&ctx, main_struct);
}

int	if_no_heredoc(t_redirector *parser)
{
	t_redirector	*current;

	current = parser;
	while (current)
	{
		if (current->token_enum == TOKEN_HEREDOC)
			return (0);
		current = current->next;
	}
	return (1);
}

int	prep_control(t_parser *parser, t_main_struct *main_struct)
{
	int	prep;

	prep = prepare_all_heredocs(parser, main_struct);
	if (prep == 130)
		return (130);
	if (prep < 0)
		return (1);
	return (0);
}

int	prepare_all_heredocs(t_parser *parser, t_main_struct *main_struct)
{
	t_parser		*cur;
	t_redirector	*r;
	int				fd;

	cur = parser;
	while (cur)
	{
		r = cur->redirector;
		while (r)
		{
			if (r->token_enum == TOKEN_HEREDOC)
			{
				fd = open_one_heredoc(r, main_struct);
				if (fd == -2)
					return (130);
				if (fd < 0)
					return (-1);
				r->herodoc_fd = fd;
			}
			r = r->next;
		}
		cur = cur->next;
	}
	return (0);
}
