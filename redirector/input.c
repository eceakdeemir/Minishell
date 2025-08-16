/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:19 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 16:52:39 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	input(t_redirector *redirector)
{
	int	fd;

	fd = open(redirector->file, O_RDONLY);
	if (fd < 0)
	{
		perror("open <");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 <");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
