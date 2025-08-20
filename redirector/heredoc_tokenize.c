/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:30:01 by igurses           #+#    #+#             */
/*   Updated: 2025/08/20 18:30:53 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	isalnum_heredoc(char *line, int i)
{
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	return (i);
}

int	heredoc_tokenize_expender_while(char *line, t_main_struct *main_struct,
		int i, int start)
{
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (line[i + 1] == 0)
				return (0);
			if (line[i + 1] == '?')
			{
				line = heredoc_combine_expender(line, i, i + 2,
						ft_itoa(main_struct->last_status));
				i = 0;
				continue ;
			}
			start = i;
			i++;
			i = isalnum_heredoc(line, i);
			line = heredoc_control_expender(start, i, *main_struct->env_struct,
					line);
			if (i - 1 != start)
				i = 0;
		}
		else
			i++;
	}
	return (1);
}

char	*heredoc_tokenize_expender(char *line, t_main_struct *main_struct)
{
	int	i;
	int	start;
	int	while_return_control;

	i = 0;
	if (!line)
		return (NULL);
	while_return_control = heredoc_tokenize_expender_while(line, main_struct, i,
			start);
	if (while_return_control == 0)
		return (NULL);
	return (line);
}
