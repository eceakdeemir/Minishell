/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:32:16 by igurses           #+#    #+#             */
/*   Updated: 2025/08/21 18:14:58 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	main_redirector(t_parser *parser, int control_value)
{
	if (parser && parser->redirector)
		control_value = redirector_funct(parser);
	if (control_value == -1)
		return (-1);
	return (0);
}

void	this_is_not_built_in_fork(char **cmd, t_main_struct *main_struct,
		t_parser *parser, pid_t pid)
{
	if (pid == 0)
	{
		reset_signals();
		this_is_not_built_in(cmd, main_struct, parser);
	}
	else if (pid < 0)
	{
		perror("Fork failed");
		ft_exit(1);
	}
}
