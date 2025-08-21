/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:32:20 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 02:22:32 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*check_is_path(char **argv, char *cmd)
{
	int		i;
	char	*path_final;
	char	*path_split;

	i = 0;
	while (argv[i])
	{
		path_split = ft_strjoin(argv[i], "/");
		path_final = ft_strjoin(path_split, cmd);
		if (access(path_final, F_OK | X_OK) == 0)
			return (path_final);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, t_main_struct *main_struct)
{
	t_enviroment	*tmp;
	char			**argv;
	char			*result;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	tmp = *(main_struct->env_struct);
	while (tmp && ft_strcmp(tmp->key, "PATH") != 0)
		tmp = tmp->next;
	if (!tmp || !tmp->value || tmp->value[0] == '\0')
		return (NULL);
	argv = ft_split(tmp->value, ':');
	if (!argv)
		return (NULL);
	result = check_is_path(argv, cmd);
	return (result);
}

static int	ft_envsize(t_enviroment *env_head)
{
	t_enviroment	*it;
	int				i;

	it = env_head;
	i = 0;
	while (it)
	{
		i++;
		it = it->next;
	}
	return (i);
}

void	env_converter_to_execve(t_main_struct *main_struct,
		char ***temporary_execve_env)
{
	int				i;
	t_enviroment	*temp;
	char			*tmp_join;

	i = 0;
	temp = *(main_struct->env_struct);
	(*temporary_execve_env) = memory_malloc(sizeof(char *)
			* (ft_envsize(*(main_struct->env_struct)) + 1));
	(*temporary_execve_env)[ft_envsize(*(main_struct->env_struct))] = NULL;
	while (i < ft_envsize(*(main_struct->env_struct)))
	{
		(*temporary_execve_env)[i] = ft_strdup(temp->key);
		tmp_join = ft_strjoin((*temporary_execve_env)[i], "=");
		(*temporary_execve_env)[i] = ft_strjoin(tmp_join, temp->value);
		temp = temp->next;
		i++;
	}
}
