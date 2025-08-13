/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:22 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/13 16:37:23 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../libraries/minishell.h"


int	output(t_redirector *redirector)
{
	int fd;

	fd = open(redirector->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open >");
		return(-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 >");
		close(fd);
		return (-1);
	}
	close(fd);
	return(0);
}