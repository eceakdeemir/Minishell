/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:47 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/20 18:34:35 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

static char	*lexeme(t_lexer *lexer)
{
	if (!lexer)
		return ("newline");
	if (lexer->token_enum == TOKEN_INPUT)
		return ("<");
	if (lexer->token_enum == TOKEN_OUTPUT)
		return (">");
	if (lexer->token_enum == TOKEN_HEREDOC)
		return ("<<");
	if (lexer->token_enum == TOKEN_APPEND)
		return (">>");
	if (lexer->token_enum == TOKEN_PIPE)
		return ("|");
	if (lexer->word != NULL && lexer->word[0] != '\0')
		return (lexer->word);
	return ("newline");
}

static void	print_syn_err(char *argv)
{
	ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(argv, 2);
	ft_putendl_fd("'", 2);
}

int	check_redirector_error(t_lexer **lexer)
{
	t_lexer	*current;
	t_lexer	*nxt;

	current = *lexer;
	while (current)
	{
		if (is_redir(current->token_enum))
		{
			nxt = current->next;
			if (!nxt || nxt->token_enum != TOKEN_WORD)
			{
				print_syn_err(lexeme(nxt));
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	print_pipe_error(t_lexer *lexer)
{
	t_lexer	*temp;

	temp = lexer;
	if (temp->token_enum == TOKEN_PIPE)
	{
		if (!temp->next)
		{
			ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
			return (1);
		}
		if (temp->next->token_enum == TOKEN_PIPE)
		{
			ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
			return (1);
		}
	}
	return (0);
}

int	check_pipe_error(t_lexer *lexer)
{
	t_lexer	*temp;
	int		return_pipe_error;

	temp = lexer;
	if (temp && (temp->token_enum == TOKEN_PIPE))
	{
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	while (temp)
	{
		return_pipe_error = print_pipe_error(temp);
		if (return_pipe_error)
			return (1);
		temp = temp->next;
	}
	return (0);
}
