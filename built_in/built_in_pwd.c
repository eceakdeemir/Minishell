
#include "../libraries/minishell.h"

int	built_in_pwd(t_parser *parser)
{
	char	cwd[4096];

	(void)parser;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_putendl_fd(cwd, 1);
	else
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		perror("");
		return (1);
	}
	return (0);
}
