#include "../libraries/built_in.h"

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

void	del_node(t_enviroment *env_node)
{
	free(env_node->key);
	free(env_node->value);
	free(env_node);
}

void	compare_key(t_enviroment *tmp, t_enviroment *current,
		t_enviroment **env)
{
	if (tmp == NULL)
		*env = current->next;
	else
		tmp->next = current->next;
	del_node(current);
}

int	built_in_unset(t_parser *parser, t_enviroment **env)
{
	t_enviroment	*tmp;
	t_enviroment	*current;
	int				i;

	tmp = NULL;
	i = 1;
	current = *env;
	while (parser->args[i] && parser->args)
	{
		if (!is_valid_key(parser->args[i]))
			return (1);
		while (current)
		{
			if (ft_strncmp(parser->args[i], current->key,
					ft_strlen(current->key)) == 0)
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
