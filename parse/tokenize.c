/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:07 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 16:49:44 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	tokenize_heredoc_and_append(t_lexer **head, char *line, int *i)
{
	if (line[(*i)] == '<' && line[(*i) + 1] == '<')
	{
		add_new_node_to_list(create_new_lexer_node("<<", TOKEN_HEREDOC), head);
		(*i) += 2;
	}
	else if (line[(*i)] == '>' && line[(*i) + 1] == '>')
	{
		add_new_node_to_list(create_new_lexer_node(">>", TOKEN_APPEND), head);
		(*i) += 2;
	}
}

void	tokenize_input_output_and_pipe(t_lexer **head, char *line, int *i)
{
	if (line[(*i)] == '|')
	{
		add_new_node_to_list(create_new_lexer_node("|", TOKEN_PIPE), head);
		(*i) += 1;
	}
	else if (line[(*i)] == '>')
	{
		add_new_node_to_list(create_new_lexer_node(">", TOKEN_OUTPUT), head);
		(*i) += 1;
	}
	else if (line[(*i)] == '<')
	{
		add_new_node_to_list(create_new_lexer_node("<", TOKEN_INPUT), head);
		(*i) += 1;
	}
}
