/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:50 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 07:52:12 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

static void	control_is_quotes(t_lexer *tmp, t_main_struct *main_struct)
{
	main_struct->i_for_tokenize++;
	while (tmp->word[main_struct->i_for_tokenize]
		&& tmp->word[main_struct->i_for_tokenize] != '\'')
		main_struct->i_for_tokenize++;
	if (tmp->word[main_struct->i_for_tokenize] == '\'')
		main_struct->i_for_tokenize++;
}

void	helper_for_query_if(t_lexer *tmp, t_main_struct *main_struct)
{
	if (tmp->word[main_struct->i_for_tokenize + 1] == '?')
		helper_for_query(main_struct, tmp);
}

static void	while_tokenize_expander(t_lexer *tmp, t_main_struct *main_struct,
		t_lexer **head, t_lexer *tmp_prev)
{
	t_lexer	*export_last;
	t_lexer	**export_head;

	while (tmp->word[main_struct->i_for_tokenize])
	{
		if (tmp->word[main_struct->i_for_tokenize] == '\'')
			control_is_quotes(tmp, main_struct);
		else if (tmp->word[main_struct->i_for_tokenize] == '$')
		{
			if (tmp->word[main_struct->i_for_tokenize + 1] == 0)
				return ;
			helper_for_query_if(tmp, main_struct);
			if (tmp->word[main_struct->i_for_tokenize + 1] == '?')
				continue ;
			helper_tokenize_char(main_struct, tmp);
			if (has_it_space(tmp->word))
			{
				helper_for_space(&export_last, &export_head, head, tmp);
				control_link_list(tmp_prev, head, export_head);
			}
			control_start(main_struct);
		}
		else
			main_struct->i_for_tokenize++;
	}
}

void	tokenize_expender(t_lexer **head, t_enviroment *env,
		t_main_struct *main_struct)
{
	t_lexer	*tmp;
	t_lexer	*tmp_prev;

	tmp = *head;
	main_struct->i_for_tokenize = 0;
	tmp_prev = NULL;
	while (tmp)
	{
		main_struct->i_for_tokenize = 0;
		if (tmp->heredoc_quoted == 0)
			while_tokenize_expander(tmp, main_struct, head, tmp_prev);
		tmp_prev = tmp;
		tmp = tmp->next;
	}
}
