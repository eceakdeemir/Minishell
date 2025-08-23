/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 13:42:48 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/23 10:25:50 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	is_has_quote(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

void	decide_heredoc_quoted(t_lexer *head)
{
	t_lexer	*cur;

	cur = head;
	while (cur)
	{
		if (cur->token_enum == TOKEN_HEREDOC && cur->next)
			cur->next->heredoc_quoted = is_has_quote(cur->next->word);
		cur = cur->next;
	}
}

void	heredoc_combine_expender(char **line, int start, int end,
		char *control_value)
{
	char	*first_part;
	char	*second_part;

	first_part = ft_substr((*line), 0, start);
	second_part = ft_substr((*line), end, ft_strlen((*line)) - end + 1);
	if (control_value)
	{
		(*line) = ft_strjoin(first_part, control_value);
		(*line) = ft_strjoin((*line), second_part);
	}
	else
		(*line) = ft_strjoin(first_part, second_part);
}

void	heredoc_control_expender(int start, int end, t_enviroment *env,
		char **line)
{
	char	*is_this_expender;
	char	*control_value;

	if ((*line)[start] == '$' && end - 1 == start)
		heredoc_combine_expender(line, start, end - 1, NULL);
	else
	{
		is_this_expender = ft_substr((*line), start + 1, end - start - 1);
		control_value = get_env_value(is_this_expender, env);
		heredoc_combine_expender(line, start, end, control_value);
	}
}

int	heredoc_fail_clear(t_parser *parser, t_main_struct *main_struct)
{
	t_redirector	*redirector;

	if (g_signal == 0)
		main_struct->last_status = 130;
	else
		main_struct->last_status = 1;
	g_signal = 0;
	redirector = parser->redirector;
	while (redirector)
	{
		if (redirector->token_enum == TOKEN_HEREDOC)
			redirector->herodoc_fd = -1;
		redirector = redirector->next;
	}
	return (-1);
}
