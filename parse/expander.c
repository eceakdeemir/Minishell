/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:50 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/23 10:25:47 by igurses          ###   ########.fr       */
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

static void	while_tokenize_expander(t_lexer *tmp, t_main_struct *m,
		t_lexer **head, t_lexer *tmp_prev)
{
	t_helper_lexer	helper;

	while (tmp->word[m->i_for_tokenize])
	{
		if (tmp->word[m->i_for_tokenize] == '\'')
			control_is_quotes(tmp, m);
		else if (tmp->word[m->i_for_tokenize] == '$')
		{
			if (!tmp->word[m->i_for_tokenize + 1])
				return ;
			if (tmp->word[m->i_for_tokenize + 1] == '?')
			{
				helper_for_query(m, tmp);
				continue ;
			}
			helper_tokenize_char(m, tmp);
			if (has_it_space(tmp->word))
			{
				helper_for_space(&helper.export_last, &helper.export_head, tmp);
				control_link_list(tmp_prev, head, helper.export_head);
			}
		}
		else
			m->i_for_tokenize++;
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
