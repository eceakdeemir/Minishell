/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herodoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:16 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/20 17:27:18 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

extern sig_atomic_t	g_signal;

int	isalnum_heredoc(char *line, int i)
{
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	return (i);
}

int	heredoc_tokenize_expender_while(char *line, t_main_struct *main_struct,
		int i, int start)
{
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (line[i + 1] == 0)
				return (0);
			if (line[i + 1] == '?')
			{
				line = heredoc_combine_expender(line, i, i + 2,
						ft_itoa(main_struct->last_status));
				i = 0;
				continue ;
			}
			start = i;
			i++;
			i = isalnum_heredoc(line, i);
			line = heredoc_control_expender(start, i, *main_struct->env_struct,
					line);
			if (i - 1 != start)
				i = 0;
		}
		else
			i++;
	}
	return (1);
}

char	*heredoc_tokenize_expender(char *line, t_main_struct *main_struct)
{
	int	i;
	int	start;
	int	while_return_control;

	i = 0;
	if (!line)
		return (NULL);
	while_return_control = heredoc_tokenize_expender_while(line, main_struct, i,
			start);
	if (while_return_control == 0)
		return (NULL);
	return (line);
}

static void	heredoc_child_process(char *limiter, t_main_struct *main_struct,
		int hd_no_quoted, int write_fd)
{
	char	*line;

	setup_signals(HEREDOC_MODE);
	while (1)
	{
		line = mem_absorb(readline("> "));
		if (!line)
			break ;
		if (!hd_no_quoted)
			line = heredoc_tokenize_expender(line, main_struct);
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
			setup_signals(INTERACTIVE_MODE);
			if (redir->herodoc_fd == -1 || g_signal == SIGINT)
				return (heredoc_fail_clear(parser, main_struct));
		}
		redir = redir->next;
	}
	return (0);
}
