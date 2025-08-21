/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:47 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 18:38:16 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	compact_args(char **str)
{
	int	first;
	int	last;

	if (!str)
		return ;
	first = 0;
	last = 0;
	while (str[first])
	{
		if (str[first][0] != '\0')
			str[last++] = str[first];
		first++;
	}
	str[last] = NULL;
}

void	compact_all_commands(t_parser *head)
{
	t_parser	*current;

	current = head;
	while (current)
	{
		compact_args(current->args);
		current = current->next;
	}
}

void	message_error2(const char *name, const char *message)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd((char *)message, 2);
}

int	has_slash(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

int	is_redir(t_token_enum token)
{
	return (token == TOKEN_INPUT || token == TOKEN_OUTPUT
		|| token == TOKEN_HEREDOC || token == TOKEN_APPEND);
}
