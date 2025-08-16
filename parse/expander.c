/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:50 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 17:28:35 by ecakdemi         ###   ########.fr       */
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
	tmp_head = mem_calloc(sizeof(t_lexer *), 1);
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
	char	*is_this_expender;
	char	*control_value;

	if (tmp->word[start] == '$' && ((end - 1) == start))
		combine_expender(tmp, start, end - 1, NULL);
	else
	{
		is_this_expender = ft_substr(tmp->word, start + 1, end - start - 1);
		control_value = get_env_value(is_this_expender, env);
		combine_expender(tmp, start, end, control_value);
	}
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

void	tokenize_expender(t_lexer **head, t_enviroment *env,
		t_main_struct *main_struct)
{
	t_lexer	*tmp;
	int		i;
	int		start;
	char	*return_val;
	t_lexer	*tmp_prev;
	t_lexer	*export_last;
	t_lexer	**export_head;

	tmp = *head;
	i = 0;
	start = 0;
	return_val = NULL;
	tmp_prev = NULL;
	while (tmp)
	{
		i = 0;
		if (tmp->heredoc_quoted == 0)
		{
			while (tmp->word[i])
			{
				if (tmp->word[i] == '\'')
				{
					i++;
					while (tmp->word[i] && tmp->word[i] != '\'')
						i++;
					if (tmp->word[i] == '\'')
						i++;
				}
				else if (tmp->word[i] == '$')
				{
					if (tmp->word[i + 1] == 0)
						return ;
					if (tmp->word[i + 1] == '?')
					{
						return_val = ft_itoa(main_struct->last_status);
						combine_expender(tmp, i, i + 2, return_val);
						i = 0;
						continue ;
					}
					start = i;
					i++;
					while (tmp->word[i] && (ft_isalnum(tmp->word[i])
							|| tmp->word[i] == '_'))
						i++;
					control_expender(start, i, env, tmp);
					if (has_it_space(tmp->word))
					{
						export_head = split_and_add(tmp);
						export_last = export_last_func(export_head);
						export_last->next = tmp->next;
						if (tmp_prev == NULL)
							*head = *export_head;
						else
							tmp_prev->next = *export_head;
					}
					if (i - 1 != start)
						i = 0;
				}
				else
					i++;
			}
		}
		tmp_prev = tmp;
		tmp = tmp->next;
	}
}
