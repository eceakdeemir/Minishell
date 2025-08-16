/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:31 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 12:26:17 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	handle_sigint_interactive(int signo)
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

void	handle_sigint_heredoc(int signo)
{
    (void)signo;
    g_signal = SIGINT;
    close(STDIN_FILENO);
    write(STDOUT_FILENO, "\n", 1);
}