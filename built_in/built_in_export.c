/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:31:06 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 17:58:42 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	print_export_error(char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

int	is_valid_env_name(char *s)
{
	int	i;

	if (!s || s[0] == '\0')
		return (1);
	if (!ft_isalpha(s[0]) && s[0] != '_')
	{
		print_export_error(s);
		return (0);
	}
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
		{
			print_export_error(s);
			return (0);
		}
		i++;
	}
	return (1);
}

int	print_export(t_enviroment *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->key, 1);
		if (env->value != NULL && env->value[0] != '\0')
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		env = env->next;
	}
	return (0);
}

void	add_or_update_env(char *str, t_enviroment **env)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(str, '=');
	if (equal_sign)
	{
		key = extract_key(str);
		value = extract_value(str);
		update_env_value(key, value, env);
	}
	else
	{
		if (!get_env_value(str, *env))
			update_env_value(str, "", env);
	}
}

int	built_in_export(t_parser *parser, t_enviroment **env)
{
	int	i;

	i = 1;
	if (!parser->args[1])
		return (print_export(*env));
	while (parser->args[i])
	{
		if (!is_valid_env_name(parser->args[i]))
			return (1);
		add_or_update_env(parser->args[i], env);
		i++;
	}
	return (0);
}
