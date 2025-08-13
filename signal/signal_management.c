/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:34 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/13 16:37:35 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

static void	set_signal(int sig, void (*handler)(int))
{
	signal(sig, handler);
}

void	setup_signals_interactive(void)
{
    set_signal(SIGINT, handle_sigint_interactive);
    set_signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_executing(void)
{
    set_signal(SIGINT, SIG_IGN);
    set_signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_heredoc(void)
{
    set_signal(SIGINT, handle_sigint_heredoc);
    set_signal(SIGQUIT, SIG_IGN);
}

void	reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}