/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:30:56 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 15:26:40 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

static int	echo_check_n(char *str)
{
	int	i;

	i = 1;
	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	built_in_echo(t_parser *parser)
{
	int	i;
	int	n;

	i = 1;
	n = 1;
	while (parser->args[i] && echo_check_n(parser->args[i]))
	{
		n = 0;
		i++;
	}
	while (parser->args[i])
	{
		ft_putstr_fd(parser->args[i], 1);
		if (parser->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (n)
		ft_putchar_fd('\n', 1);
	return (0);
}
