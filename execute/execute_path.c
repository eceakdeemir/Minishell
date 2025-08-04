#include "../libraries/execute.h"
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
		free(path_split);
		if (access(path_final, F_OK | X_OK) == 0)
		{
			free_argv(argv);
			return (path_final);
		}
		free(path_final);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, char **env)
{
	char **argv;
	char *result;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}

	while (*env && ft_strncmp(*env, "PATH", 4))
		env++;
	argv = ft_split(*env + 5, ':');
	result = check_is_path(argv, cmd);
	return (result);
}