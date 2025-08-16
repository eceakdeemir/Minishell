/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:31:19 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 16:00:43 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*extract_key(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (ft_strndup(str, i));
}

char	*extract_value(char *str)
{
	char	*equal;

	equal = ft_strchr(str, '=');
	if (!equal)
		return (ft_strdup(""));
	return (ft_strdup(equal + 1));
}

char	*get_env_value(const char *key, t_enviroment *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

void	update_env_value(const char *key, const char *value, t_enviroment **env)
{
	t_enviroment	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	add_new_node_for_env(env, create_new_node_for_env(ft_strdup(key),
			ft_strdup(value)));
}

void	init_env(char **envp, t_enviroment **env_list)
{
	t_enviroment	*new_node;
	char			*key;
	char			*value;
	int				i;

	i = 0;
	while (envp[i])
	{
		key = extract_key(envp[i]);
		value = extract_value(envp[i]);
		new_node = create_new_node_for_env(key, value);
		add_new_node_for_env(env_list, new_node);
		i++;
	}
}
