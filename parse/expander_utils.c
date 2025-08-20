/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 21:19:12 by igurses           #+#    #+#             */
/*   Updated: 2025/08/20 21:21:26 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	helper_tokenize_char(int start, t_main_struct *main_struct,
		t_lexer *tmp)
{
	start = main_struct->i_for_tokenize;
	main_struct->i_for_tokenize++;
	while (tmp->word[main_struct->i_for_tokenize]
		&& (ft_isalnum(tmp->word[main_struct->i_for_tokenize])
			|| tmp->word[main_struct->i_for_tokenize] == '_'))
		main_struct->i_for_tokenize++;
	control_expender(start, main_struct->i_for_tokenize,
		*(main_struct->env_struct), tmp);
}

void	helper_for_query(t_main_struct *main_struct, t_lexer *tmp,
		char *return_val)
{
	return_val = ft_itoa(main_struct->last_status);
	combine_expender(tmp, main_struct->i_for_tokenize,
		main_struct->i_for_tokenize + 2, return_val);
	main_struct->i_for_tokenize = 0;
}

void	helper_for_space(t_lexer *export_last, t_lexer **export_head,
		t_lexer **head, t_lexer *tmp)
{
	export_head = split_and_add(tmp);
	export_last = export_last_func(export_head);
	export_last->next = tmp->next;
}

void	control_start(t_main_struct *main_struct, int start)
{
	if (main_struct->i_for_tokenize - 1 != start)
		main_struct->i_for_tokenize = 0;
}

void	control_link_list(t_lexer *tmp_prev, t_lexer **head,
		t_lexer **export_head)
{
	if (tmp_prev == NULL)
		*head = *export_head;
	else
		tmp_prev->next = *export_head;
}
