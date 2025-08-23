/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 21:19:12 by igurses           #+#    #+#             */
/*   Updated: 2025/08/23 10:25:43 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

void	helper_tokenize_char(t_main_struct *m, t_lexer *tmp)
{
	int		start;
	int		var_end;
	size_t	new_len;

	start = m->i_for_tokenize;
	m->i_for_tokenize++;
	while (tmp->word[m->i_for_tokenize]
		&& (ft_isalnum(tmp->word[m->i_for_tokenize])
			|| tmp->word[m->i_for_tokenize] == '_'))
		m->i_for_tokenize++;
	var_end = m->i_for_tokenize;
	if (var_end == start + 1)
		return ;
	control_expender(start, var_end, *(m->env_struct), tmp);
	new_len = ft_strlen(tmp->word);
	if ((size_t)m->i_for_tokenize > new_len)
		m->i_for_tokenize = (int)new_len;
}

void	helper_for_query(t_main_struct *m, t_lexer *tmp)
{
	int		start;
	char	*ret;
	size_t	len;

	start = m->i_for_tokenize;
	ret = ft_itoa(m->last_status);
	if (!ret)
		return ;
	combine_expender(tmp, start, start + 2, ret);
	len = 0;
	while (tmp->word[start + len] && ft_isdigit(tmp->word[start + len]))
		len++;
	m->i_for_tokenize = start + (int)len;
}

void	helper_for_space(t_lexer **export_last, t_lexer ***export_head,
		t_lexer *tmp)
{
	*export_head = split_and_add(tmp);
	*export_last = export_last_func(*export_head);
	(*export_last)->next = tmp->next;
}

void	control_start(t_main_struct *main_struct)
{
	if ((main_struct->i_for_tokenize - 1) != 0)
		main_struct->i_for_tokenize = 0;
}

void	control_link_list(t_lexer *tmp_prev, t_lexer **head,
		t_lexer **export_head)
{
	if (tmp_prev == NULL && *export_head)
		*head = *export_head;
	else
		tmp_prev->next = *export_head;
}
