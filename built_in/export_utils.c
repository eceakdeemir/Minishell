#include "../libraries/minishell.h"

t_lexer	*export_last_func(t_lexer **head)
{
	t_lexer	*node;

	node = *head;
	while (node->next)
		node = node->next;
	return (node);
}

int	has_it_space(char *str)
{
	int	i;

	i = 0;
	if (str[i] == ' ')
		return (1);
	while (str[i])
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}