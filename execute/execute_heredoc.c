/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:41:55 by igurses           #+#    #+#             */
/*   Updated: 2025/08/20 18:51:43 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	heredoc_count(t_parser *parse_node)
{
	int				i;
	t_redirector	*tmp;

	i = 0;
	tmp = parse_node->redirector;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	write_for_main_heredoc_child_process(int pipefd, char *line)
{
	write(pipefd, line, ft_strlen(line));
	write(pipefd, "\n", 1);
}

void	child_pid_control(t_main_struct *main_struct, pid_t pid,
		t_parser *current)
{
	if (!current->next)
		main_struct->last_child_pid = pid;
}

void	close_fd_and_child_control(t_parser *current, pid_t pid,
		t_main_struct *main_struct)
{
	close_heredoc_fds(current);
	child_pid_control(main_struct, pid, current);
}

void	close_heredoc_fds(t_parser *current)
{
	t_redirector	*r;

	r = current->redirector;
	while (r)
	{
		if (r->token_enum == TOKEN_HEREDOC && r->herodoc_fd >= 0)
		{
			close(r->herodoc_fd);
			r->herodoc_fd = -1;
		}
		r = r->next;
	}
}
