/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:26 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/18 14:34:40 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	redirector_funct(t_parser *parser)
{
	t_redirector	*redirector_tmp;
	int				last_in_fd;
	int				return_value;

	last_in_fd = -1;
	return_value = 0;
	redirector_tmp = parser->redirector;
	while (redirector_tmp)
	{
		if (redirector_tmp->token_enum == TOKEN_APPEND)
			return_value = append(redirector_tmp);
		else if (redirector_tmp->token_enum == TOKEN_OUTPUT)
			return_value = output(redirector_tmp);
		else if (redirector_tmp->token_enum == TOKEN_INPUT)
			return_value = input(redirector_tmp);
		else if (redirector_tmp->token_enum == TOKEN_HEREDOC)
			last_in_fd = redirector_tmp->herodoc_fd;
		if (return_value == -1)
			return (-1);
		redirector_tmp = redirector_tmp->next;
	}
	if (last_in_fd != -1)
		dup2(last_in_fd, STDIN_FILENO);
	return (0);
}
