/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:31:11 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 15:44:05 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	print_unset_error(char *str)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

int	is_valid_key(char *key)
{
	if (!ft_isalpha(key[0]))
	{
		print_unset_error(key);
		return (0);
	}
	return (1);
}

void	compare_key(t_enviroment *tmp, t_enviroment *current,
		t_enviroment **env)
{
	if (tmp == NULL)
		*env = current->next;
	else
		tmp->next = current->next;
}

int	built_in_unset(t_parser *parser, t_enviroment **env)
{
	t_enviroment	*tmp;
	t_enviroment	*current;
	int				i;

	i = 1;
	while (parser->args[i])
	{
		tmp = NULL;
		current = *env;
		if (!is_valid_key(parser->args[i]))
			return (1);
		while (current)
		{
			if (ft_strcmp(parser->args[i], current->key) == 0)
			{
				compare_key(tmp, current, env);
				break ;
			}
			tmp = current;
			current = current->next;
		}
		i++;
	}
	return (0);
}
