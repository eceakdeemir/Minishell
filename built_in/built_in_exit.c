/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:31:03 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 15:38:07 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

static int	check_is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	normalize_exit_code(t_parser *parser)
{
	int	main_number;
	int	numeric;

	numeric = ft_atoi(parser->args[1]);
	main_number = (int)(numeric % 256);
	if (main_number < 0)
		main_number += 256;
	return (main_number);
}

int	built_in_exit(t_parser *parser, t_main_struct *main_struct)
{
	int	exit_code;

	ft_putendl_fd("exit", 1);
	if (!parser->args[1])
		ft_exit(0);
	if (!check_is_numeric(parser->args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(parser->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		ft_exit(2);
	}
	if (parser->args[2])
	{
		ft_putendl_fd("minishell: exit: : too many arguments", 2);
		return (1);
	}
	exit_code = normalize_exit_code(parser);
	main_struct->last_status = exit_code;
	ft_exit(exit_code);
	return (0);
}
