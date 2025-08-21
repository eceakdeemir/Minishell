/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:01 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 18:42:36 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"
#include <stdio.h>

void	add_redirector(t_parser *cmd, t_token_enum type, char *file,
		int hd_no_expand)
{
	t_redirector	*new;
	t_redirector	*current;

	new = memory_malloc(sizeof(t_redirector));
	new->token_enum = type;
	new->file = ft_strdup(file);
	new->hd_no_expand = hd_no_expand;
	new->herodoc_fd = -1;
	new->next = NULL;
	if (!cmd->redirector)
		cmd->redirector = new;
	else
	{
		current = cmd->redirector;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

t_parser	*create_new_parser_node(t_lexer **lexer)
{
	t_parser	*cmd;
	int			count;

	cmd = memory_calloc(1, sizeof(t_parser));
	count = count_args(*lexer);
	cmd->args = memory_malloc(sizeof(char *) * (count + 1));
	if (!cmd->args)
		return (NULL);
	cmd->redirector = NULL;
	fill_args_to_parser(cmd, lexer);
	return (cmd);
}

void	fill_args_to_parser(t_parser *cmd, t_lexer **lexer)
{
	int	i;

	i = 0;
	while (*lexer && (*lexer)->token_enum != TOKEN_PIPE)
	{
		if (*lexer && (*lexer)->token_enum == TOKEN_WORD)
		{
			cmd->args[i++] = ft_strdup((*lexer)->word);
			*lexer = (*lexer)->next;
		}
		else
		{
			if ((*lexer)->token_enum == TOKEN_HEREDOC && (*lexer)->next)
				add_redirector(cmd, (*lexer)->token_enum, (*lexer)->next->word,
					(*lexer)->next->heredoc_quoted);
			else
				add_redirector(cmd, (*lexer)->token_enum, (*lexer)->next->word,
					0);
			*lexer = (*lexer)->next;
			*lexer = (*lexer)->next;
		}
	}
	cmd->args[i] = NULL;
}

t_parser	*main_parser_func(t_lexer *lexer)
{
	t_parser	*head;
	t_parser	*current;
	t_parser	*cmd;

	head = NULL;
	current = NULL;
	while (lexer)
	{
		cmd = create_new_parser_node(&lexer);
		if (!cmd)
			return (NULL);
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;
		if (lexer && lexer->token_enum == TOKEN_PIPE)
			lexer = lexer->next;
	}
	return (head);
}

t_parser	*parser_funct(t_lexer **head, t_enviroment **env_struct,
		t_main_struct *main_struct)
{
	t_parser	*parser;

	if (check_pipe_error(*head) || check_redirector_error(head))
	{
		main_struct->last_status = 2;
		return (NULL);
	}
	decide_heredoc_quoted(*head);
	tokenize_expender(head, *env_struct, main_struct);
	remove_quotes_all(head);
	parser = main_parser_func(*head);
	if (!parser)
		return (NULL);
	return (parser);
}
