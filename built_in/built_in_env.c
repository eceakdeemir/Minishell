
#include "../libraries/minishell.h"

int	built_in_env(t_enviroment *env)
{
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}
