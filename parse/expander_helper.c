/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 19:13:46 by igurses           #+#    #+#             */
/*   Updated: 2025/08/21 19:05:18 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

t_lexer	**split_and_add(t_lexer *node)
{
	t_lexer	**tmp_head;
	char	*word;
	int		i;
	int		start;

	i = 0;
	tmp_head = memory_calloc(sizeof(t_lexer *), 1);
	if (!tmp_head)
		return (NULL);
	while (node->word[i])
	{
		while (node->word[i] == ' ')
			i++;
		start = i;
		while (node->word[i] && node->word[i] != ' ')
			i++;
		word = ft_substr(node->word, start, i);
		add_new_node_to_list(create_new_lexer_node(word, TOKEN_WORD), tmp_head);
	}
	return (tmp_head);
}

void	combine_expender(t_lexer *tmp, int start, int end, char *control_value)
{
	char	*first_part;
	char	*second_part;

	first_part = ft_substr(tmp->word, 0, start);
	second_part = ft_substr(tmp->word, end, ft_strlen(tmp->word) - end + 1);
	if (control_value)
	{
		tmp->word = ft_strjoin(first_part, control_value);
		tmp->word = ft_strjoin(tmp->word, second_part);
	}
	else
		tmp->word = ft_strjoin(first_part, second_part);
}

void	control_expender(int start, int end, t_enviroment *env, t_lexer *tmp)
{
	char	*name;
	char	*val;
	int		len;

	len = end - (start + 1);
	if (len <= 0)
		return ;
	name = ft_substr(tmp->word, start + 1, len);
	if (!name)
		return ;
	val = get_env_value(name, env);
	if (val)
		combine_expender(tmp, start, end, ft_strdup(val));
	else
		combine_expender(tmp, start, end, ft_strdup(""));
}

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
