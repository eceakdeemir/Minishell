/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:31:09 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 15:42:48 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	built_in_pwd(t_parser *parser)
{
	char	cwd[4096];

	(void)parser;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_putendl_fd(cwd, 1);
	else
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		perror("");
		return (1);
	}
	return (0);
}
