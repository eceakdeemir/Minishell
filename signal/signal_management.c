/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:34 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/23 10:26:26 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

static void	set_signal(int sig, void (*handler)(int))
{
	signal(sig, handler);
}

void	signals_interactive(void)
{
	set_signal(SIGINT, sigint_interactive);
	set_signal(SIGQUIT, SIG_IGN);
}

void	signals_executing(void)
{
	set_signal(SIGINT, SIG_IGN);
	set_signal(SIGQUIT, SIG_IGN);
}

void	signals_heredoc(void)
{
	set_signal(SIGINT, sigint_heredoc);
	set_signal(SIGQUIT, SIG_IGN);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
