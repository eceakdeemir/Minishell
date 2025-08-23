/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:30:07 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/23 10:24:39 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

static int	arg_count(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

char	*get_target_directory(t_parser *parser, t_enviroment **env)
{
	char	*home;
	int		arg_len;

	arg_len = 0;
	while (parser->args && parser->args[arg_len])
		arg_len++;
	if (arg_len > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (NULL);
	}
	if (arg_len == 1 && env)
	{
		home = get_env_value("HOME", (*env));
		if (!home)
			ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (home);
	}
	return (parser->args[1]);
}

void	update_cd_env(t_enviroment **env, char *old_pwd)
{
	char	cwd[4096];

	if (old_pwd)
		update_env_value("OLDPWD", old_pwd, env);
	if (getcwd(cwd, sizeof(cwd)))
		update_env_value("PWD", cwd, env);
}

int	built_in_cd(t_parser *parser, t_enviroment **env)
{
	char	*old_pwd;
	char	*target_directory;

	if (arg_count(parser->args) > 2)
	{
		ft_putendl_fd("too many arguments", 2);
		return (1);
	}
	target_directory = get_target_directory(parser, env);
	if (!target_directory)
		return (1);
	old_pwd = get_env_value("PWD", *env);
	if (chdir(target_directory) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(target_directory);
		return (1);
	}
	update_cd_env(env, old_pwd);
	return (0);
}
