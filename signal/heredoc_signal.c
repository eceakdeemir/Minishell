/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:36:11 by igurses           #+#    #+#             */
/*   Updated: 2025/08/23 10:26:18 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	set_sig(int sig, void (*h)(int))
{
	struct sigaction	sa;

	sa.sa_handler = h;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(sig, &sa, NULL);
}

void	enter_heredoc_parent_mode(void)
{
	set_sig(SIGINT, SIG_IGN);
	set_sig(SIGQUIT, SIG_IGN);
}

void	restore_interactive_mode(void)
{
	set_sig(SIGINT, sigint_interactive);
	set_sig(SIGQUIT, SIG_IGN);
}
