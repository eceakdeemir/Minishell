/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:30:07 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/13 15:30:08 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/built_in.h"
#include "../libraries/minishell.h"
#include "../libraries/enviroment.h"


char *get_target_directory(t_parser *parser, t_enviroment **env) //
{
	if (parser->args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (NULL);
	}
	if (!parser->args[1])
	{
		char *home = get_env_value("HOME", *env);
		if (!home)
			ft_putendl_fd("minishell: cd: HOME not set", 2);
		return home;
	}
	return (parser->args[1]);
}

void update_cd_env(t_enviroment **env, char *old_pwd)
{
	char cwd[4096]; //current working directory

	if (old_pwd)
		update_env_value("OLDPWD", old_pwd, env);
	if (getcwd(cwd, sizeof(cwd))) //o anki çalışma dizininin tam pathini verir.
		update_env_value("PWD", cwd, env);
}

int built_in_cd(t_parser *parser, t_enviroment **env)
{
	char *target_directory; //gitmek istediğin hedef dizin
	char *old_pwd;

	old_pwd = get_env_value("PWD", *env); //pwd oldpwdde tutulacak, pwd değişecek.
	if (parser->args[2]) //cd 1 argüman almalı.
	{
		ft_putendl_fd(" too many arguments", 2);
		return (1);
	}
	target_directory = get_target_directory(parser, env);
	if (!target_directory)
		return (1);
	old_pwd = get_env_value("PWD", *env);
	if (chdir(target_directory) != 0)//chdir çalıştığı için başarılı olduğunda dizin değişmiş olacak. düzgün çalıştığında 0 döndürür
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(target_directory); //sistemden gelen hata mesajını direkt eklemek için
		return (1);
	}
	update_cd_env(env, old_pwd);
	return (0);
}



