/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:37 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/13 16:37:38 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

sig_atomic_t		g_signal = 0;

void	setup_signals(int context)
{
	if (context == INTERACTIVE_MODE)
		setup_signals_interactive();
	else if (context == EXECUTING_MODE)
		setup_signals_executing();
	else if (context == HEREDOC_MODE)
		setup_signals_heredoc();
}

