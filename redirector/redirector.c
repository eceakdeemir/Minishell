/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:26 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 16:53:28 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	redirector_funct(t_parser *parser)
{
	t_redirector	*redirector;
	int				last_in_fd;
	int				return_value;

	last_in_fd = -1;
	return_value = 0;
	redirector = parser->redirector;
	while (redirector)
	{
		if (redirector->token_enum == TOKEN_APPEND)
			return_value = append(redirector);
		else if (redirector->token_enum == TOKEN_OUTPUT)
			return_value = output(redirector);
		else if (redirector->token_enum == TOKEN_INPUT)
			return_value = input(redirector);
		else if (redirector->token_enum == TOKEN_HEREDOC)
			last_in_fd = redirector->herodoc_fd;
		if (return_value == -1)
			return (-1);
		redirector = redirector->next;
	}
	if (last_in_fd != -1)
		dup2(last_in_fd, STDIN_FILENO);
	return (0);
}
