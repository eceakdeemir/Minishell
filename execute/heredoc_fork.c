/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:45:03 by igurses           #+#    #+#             */
/*   Updated: 2025/08/23 10:25:05 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	open_one_heredoc(t_redirector *r, t_main_struct *main_struct)
{
	int		fd[2];
	pid_t	pid;
	int		status;
	int		return_parent;
	int		return_child;

	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	enter_heredoc_parent_mode();
	pid = fork();
	return_child = open_one_heredoc_child(pid, r, main_struct, fd);
	if (return_child < 0)
		return (return_child);
	return_parent = open_one_heredoc_parent(pid, fd, main_struct, status);
	if (return_parent < 0)
		return (return_parent);
	return (fd[0]);
}

void	process_heredoc_line(char **line, int hd_no_expand,
		t_main_struct *main_struct)
{
	if (!hd_no_expand)
		heredoc_tokenize_expender(line, main_struct);
}

void	main_heredoc_child_process(char *limiter, t_main_struct *main_struct,
		int hd_no_expand, int pipefd)
{
	char	*line;

	setup_signals(HEREDOC);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		process_heredoc_line(&line, hd_no_expand, main_struct);
		if (ft_strcmp(line, limiter) == 0)
			break ;
		write_for_main_heredoc_child_process(pipefd, line);
	}
	close(pipefd);
	ft_exit(0);
}

int	open_one_heredoc_child(pid_t pid, t_redirector *r,
		t_main_struct *main_struct, int fd[2])
{
	if (pid < 0)
	{
		perror("fork");
		close(fd[0]);
		close(fd[1]);
		restore_interactive_mode();
		return (-1);
	}
	if (pid == 0)
	{
		main_heredoc_child_process(r->file, main_struct, r->hd_no_expand,
			fd[1]);
	}
	return (0);
}

int	open_one_heredoc_parent(pid_t pid, int fd[2], t_main_struct *main_struct,
		int status)
{
	close(fd[1]);
	if (waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid");
		close(fd[0]);
		restore_interactive_mode();
		return (-1);
	}
	restore_interactive_mode();
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		close(fd[0]);
		main_struct->last_status = 130;
		g_signal = SIGINT;
		return (-2);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0
		&& WEXITSTATUS(status) != 130)
	{
		close(fd[0]);
		return (-1);
	}
	return (0);
}
