/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:37 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/23 10:26:31 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

sig_atomic_t	g_signal = 0;

void	setup_signals(int context)
{
	if (context == INTERACTIVE)
		signals_interactive();
	else if (context == EXECUTING)
		signals_executing();
	else if (context == HEREDOC)
		signals_heredoc();
}
