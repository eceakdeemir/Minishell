/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herodoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:16 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/23 10:26:06 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

extern sig_atomic_t	g_signal;

static void	heredoc_child_process(char *limiter, t_main_struct *main_struct,
		int hd_no_quoted, int write_fd)
{
	char	*line;

	setup_signals(HEREDOC);
	while (1)
	{
		line = memory_absorb(readline("> "));
		if (!line)
			break ;
		if (!hd_no_quoted)
			heredoc_tokenize_expender(&line, main_struct);
		if (line && ft_strcmp(line, limiter) == 0)
			break ;
		ft_putendl_fd(line, write_fd);
	}
	close(write_fd);
	ft_exit(0);
}

static int	wait_child_for_heredoc(pid_t pid, int pipefd[2],
		t_main_struct *main_struct)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		main_struct->last_status = 130;
		return (-1);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (0);
}

static int	create_heredoc_file(char *limiter, t_enviroment *env,
		t_main_struct *main_struct, int hd_no_quoted)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child_process(limiter, main_struct, hd_no_quoted, pipefd[1]);
	}
	if (wait_child_for_heredoc(pid, pipefd, main_struct) == -1)
		return (-1);
	return (pipefd[0]);
}

int	prepare_heredocs(t_parser *parser, t_enviroment *env,
		t_main_struct *main_struct)
{
	t_redirector	*redir;

	redir = parser->redirector;
	while (redir)
	{
		if (redir->token_enum == TOKEN_HEREDOC)
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			redir->herodoc_fd = create_heredoc_file(redir->file, env,
					main_struct, redir->hd_no_expand);
			setup_signals(INTERACTIVE);
			if (redir->herodoc_fd == -1 || g_signal == SIGINT)
				return (heredoc_fail_clear(parser, main_struct));
		}
		redir = redir->next;
	}
	return (0);
}
