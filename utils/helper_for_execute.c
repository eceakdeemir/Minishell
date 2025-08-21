/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_for_execute.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrahim <ibrahim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:43 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 11:30:06 by ibrahim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*control_path(char **cmd, t_main_struct *main_struct)
{
	char	*path;

	if (has_slash(cmd[0]))
		path = ft_strdup(cmd[0]);
	else
		path = find_path(cmd[0], main_struct);
	if (!path)
	{
		if (has_slash(cmd[0]))
			message_error2(cmd[0], "No such file or directory");
		else
			message_error2(cmd[0], "command not found");
		ft_exit(127);
	}
	return (path);
}

void	is_acces_path(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		message_error2(path, "No such file or directory");
		ft_exit(127);
	}
	if (S_ISDIR(st.st_mode))
	{
		message_error2(path, "Is a directory");
		ft_exit(126);
	}
	if (access(path, X_OK) == -1)
	{
		if (errno == EACCES)
		{
			message_error2(path, "Permission denied");
			ft_exit(126);
		}
		message_error2(path, strerror(errno));
		ft_exit(127);
	}
}

void	exec_or_die(const char *path, char **argv, t_main_struct *main_struct)
{
	char	**envp;

	env_converter_to_execve(main_struct, &envp);
	execve(path, argv, envp);
	if (errno == EACCES)
	{
		message_error2(path, "Permission denied");
		ft_exit(126);
	}
	if (errno == ENOEXEC)
	{
		message_error2(path, "Exec format error");
		ft_exit(126);
	}
	message_error2(path, strerror(errno));
	ft_exit(127);
}
