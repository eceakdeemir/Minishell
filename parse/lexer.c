/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:53 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 02:22:32 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

t_lexer	*create_new_lexer_node(char *str, t_token_enum token)
{
	t_lexer	*new_node;

	new_node = memory_malloc(sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	new_node->word = str;
	new_node->token_enum = token;
	new_node->heredoc_quoted = 0;
	new_node->next = NULL;
	return (new_node);
}

void	add_new_node_to_list(t_lexer *node, t_lexer **head)
{
	t_lexer	*tmp;

	tmp = *head;
	if (!tmp)
	{
		*head = node;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = node;
}

void	add_word_to_list(int start, int *i, t_lexer **head, char *line)
{
	char	quote;

	while (line[*i] && !ft_isspace(line[*i]) && !ft_strchr("|<>", line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '\"')
		{
			quote = line[*i];
			(*i)++;
			while (line[*i] && line[*i] != quote)
				(*i)++;
			if (line[*i] == '\0')
			{
				ft_putendl_fd("syntax error: unclosed quote", 2);
				*head = NULL;
				return ;
			}
			(*i)++;
		}
		else
			(*i)++;
	}
	if ((*i) - start > 0)
		add_new_node_to_list(create_new_lexer_node(ft_strndup(&line[start], (*i)
					- start), TOKEN_WORD), head);
}

t_lexer	**lexer(char *line)
{
	t_lexer	**head;
	int		i;

	i = 0;
	head = memory_malloc(sizeof(t_lexer *));
	*head = NULL;
	while (line[i])
	{
		while (ft_isspace(line[i]) == 1)
			i++;
		if (((line[i] == '<' && line[i + 1] == '<') || (line[i] == '>' && line[i
						+ 1] == '>')) && line[i + 1])
		{
			tokenize_heredoc_and_append(head, line, &i);
			continue ;
		}
		else if ((line[i] == '<') || (line[i] == '>') || (line[i] == '|'))
		{
			tokenize_input_output_and_pipe(head, line, &i);
			continue ;
		}
		add_word_to_list(i, &i, head, line);
	}
	return (head);
}
