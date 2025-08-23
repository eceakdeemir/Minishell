/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tokenize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:30:01 by igurses           #+#    #+#             */
/*   Updated: 2025/08/23 10:26:02 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	isalnum_heredoc(char **line, int i)
{
	while ((*line)[i] && (ft_isalnum((*line)[i]) || (*line)[i] == '_'))
		i++;
	return (i);
}

int	heredoc_tokenize_expender_while(char **line, t_main_struct *main_struct,
		int i, int start)
{
	while ((*line)[i])
	{
		if ((*line)[i] == '$')
		{
			if ((*line)[i + 1] == 0)
				return (0);
			if ((*line)[i + 1] == '?')
			{
				heredoc_combine_expender(line, i, i + 2,
					ft_itoa(main_struct->last_status));
				i = 0;
				continue ;
			}
			start = i;
			i++;
			i = isalnum_heredoc(line, i);
			heredoc_control_expender(start, i, *main_struct->env_struct, line);
			if (i - 1 != start)
				i = 0;
		}
		else
			i++;
	}
	return (1);
}

void	heredoc_tokenize_expender(char **line, t_main_struct *main_struct)
{
	int	i;
	int	start;
	int	while_return_control;

	i = 0;
	start = 0;
	if (!line || !*line)
		return ;
	while_return_control = heredoc_tokenize_expender_while(line, main_struct, i,
			start);
	(void)while_return_control;
}
