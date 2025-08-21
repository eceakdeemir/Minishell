/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrahim <ibrahim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:31 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 14:40:49 by ibrahim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	sigint_interactive(int signo)
{
	(void)signo;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigint_executing(int signo)
{
	(void)signo;
	g_signal = SIGINT;
}

void	handle_sigquit_executing(int signo)
{
	(void)signo;
	g_signal = SIGQUIT;
}

void	sigint_heredoc(int signo)
{
	(void)signo;
	g_signal = SIGINT;
	close(STDIN_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	ft_exit(130);
}

void	heredoc_sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}
