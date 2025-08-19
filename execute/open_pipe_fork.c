/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_pipe_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:32:37 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/19 20:26:17 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

extern sig_atomic_t	g_signal;

static void	set_sig(int sig, void (*h)(int))
{
	struct sigaction	sa;

	sa.sa_handler = h;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(sig, &sa, NULL);
}

static void	enter_heredoc_parent_mode(void)
{
	set_sig(SIGINT, SIG_IGN);
	set_sig(SIGQUIT, SIG_IGN);
}

static void	restore_interactive_mode(void)
{
	set_sig(SIGINT, handle_sigint_interactive);
	set_sig(SIGQUIT, SIG_IGN);
}

void	path_found_and_execute(t_parser *current, t_main_struct *main_struct)
{
	char	*path;
	char	**temporary_execve_env;

	if (!current->args || !current->args[0])
		ft_exit(0);
	path = find_path(current->args[0], main_struct);
	if (!path)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putendl_fd(current->args[0], 2);
		ft_exit(127);
	}
	env_converter_to_execve(main_struct, &temporary_execve_env);
	execve(path, current->args, temporary_execve_env);
	perror("execve");
	ft_exit(126);
}

static int	if_no_heredoc(t_redirector *parser)
{
	t_redirector	*current;

	current = parser;
	while (current)
	{
		if (current->token_enum == TOKEN_HEREDOC)
			return (0);
		current = current->next;
	}
	return (1);
}

void	arrangement_dup(int **pipes, int pipe_count, t_parser *current, int i)
{
	int	j;

	j = 0;
	reset_signals();
	if (current->redirector && !if_no_heredoc(current->redirector))
		dup2(current->redirector->herodoc_fd, STDIN_FILENO);
	else if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < pipe_count)
		dup2(pipes[i][1], STDOUT_FILENO);
	while (j < pipe_count)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
	if (current->redirector && !if_no_heredoc(current->redirector))
		close(current->redirector->herodoc_fd);
}

static int	heredoc_count(t_parser *parse_node)
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

static char	*process_heredoc_line(char *line, int hd_no_expand,
		t_main_struct *main_struct)
{
	char	*orig;

	if (!hd_no_expand)
	{
		orig = line;
		line = heredoc_tokenize_expender(orig, *(main_struct->env_struct),
				main_struct);
	}
	return (line);
}

static void	main_heredoc_child_process(char *limiter, t_enviroment *env,
		t_main_struct *main_struct, int hd_no_expand, int pipefd)
{
	char	*line;

	setup_signals(HEREDOC_MODE);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		line = process_heredoc_line(line, hd_no_expand, main_struct);
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
		main_heredoc_child_process(r->file, *(main_struct->env_struct),
			main_struct, r->hd_no_expand, fd[1]);
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

static int	open_one_heredoc(t_redirector *r, t_main_struct *main_struct)
{
	int		fd[2];
	pid_t	pid;
	int		status;
	char	*line;
	int		delimiter_hit;
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

static int	prepare_all_heredocs(t_parser *parser, t_main_struct *main_struct)
{
	t_parser		*cur;
	t_redirector	*r;
	int				fd;

	cur = parser;
	while (cur)
	{
		r = cur->redirector;
		while (r)
		{
			if (r->token_enum == TOKEN_HEREDOC)
			{
				fd = open_one_heredoc(r, main_struct);
				if (fd == -2)
					return (130);
				if (fd < 0)
					return (-1);
				r->herodoc_fd = fd;
			}
			r = r->next;
		}
		cur = cur->next;
	}
	return (0);
}


static void	handle_child_process(t_parser *current, int **pipes, int pipe_count,
		t_main_struct *main_struct, int i)
{
	int	control_redirector;

	arrangement_dup(pipes, pipe_count, current, i);
	if (current->redirector)
	{
		control_redirector = main_redirector(current, 0);
		if (control_redirector == -1)
			ft_exit(1);
	}
	if (!current->args || !current->args[0])
	{
		if (pipe_count > 0)
			ft_exit(2);
		ft_exit(0);
	}
	if (current->built_type >= 0 && current->built_type <= 6)
		ft_exit(run_built_in(current, main_struct));
	else
		path_found_and_execute(current, main_struct);
}

static void	close_heredoc_fds(t_parser *current)
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

int	forks_and_exec_commands(t_parser *parser, int **pipes, int pipe_count,
		t_main_struct *main_struct)
{
	t_parser		*current;
	pid_t			pid;
	int				i;
	int				prep;

	prep = prepare_all_heredocs(parser, main_struct);
	if (prep == 130) /* heredoc Ctrl+C */
		return (130);
	if (prep < 0)
		return (1);
	current = parser;
	i = 0;
	while (current)
	{
		pid = fork();
		if (pid == 0)
			handle_child_process(current, pipes, pipe_count, main_struct, i);
		else if (pid < 0)
		{
			perror("fork");
			return (1);
		}
		close_heredoc_fds(current);
		if (!current->next)
			main_struct->last_child_pid = pid;
		current = current->next;
		i++;
	}
	return (0);
}

static void	handle_wait_result(pid_t pid, int status, t_main_struct *main_struct,
		t_wait_ctx *ctx, int *i)
{
	if (pid > 0)
	{
		(*i)--;
		is_there_signal_at_child(pid, status, main_struct, ctx);
	}
	else if (pid < 0)
	{
		if (errno == EINTR)
			return ;
		if (errno == ECHILD)
			*i = 0;
		else
		{
			perror("waitpid");
			*i = 0;
		}
	}
}

static void	finalize_wait_status(t_wait_ctx *ctx, t_main_struct *main_struct)
{
	int	saw_sigint_newline;

	saw_sigint_newline = 0;
	if (ctx->saw_sigint)
	{
		write(STDOUT_FILENO, "\n", 1);
		saw_sigint_newline = 1;
	}
	if (ctx->saw_sigint && !ctx->set_last)
		main_struct->last_status = 130;
	g_signal = 0;
	(void)saw_sigint_newline;
}

void	wait_all_child(int i, int status, t_main_struct *main_struct,
		t_parser *parser)
{
	pid_t		pid;
	t_wait_ctx	ctx;

	ctx.saw_sigint = 0;
	ctx.set_last = 0;
	i = count_cmd(parser);
	while (i > 0)
	{
		pid = waitpid(-1, &status, 0);
		handle_wait_result(pid, status, main_struct, &ctx, &i);
	}
	finalize_wait_status(&ctx, main_struct);
}

static int	**create_pipes(int pipe_count)
{
	int	**pipes;
	int	i;

	if (pipe_count <= 0)
		return (NULL);
	pipes = mem_malloc(sizeof(int *) * pipe_count);
	for (i = 0; i < pipe_count; i++)
	{
		pipes[i] = mem_malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return (NULL);
		}
	}
	return (pipes);
}

static void	close_all_pipes(int **pipes, int pipe_count)
{
	int	i;

	if (!pipes)
		return ;
	for (i = 0; i < pipe_count; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

void	execute_main(t_parser *parser, t_main_struct *main_struct)
{
	int	pipe_count;
	int	status;
	int	**pipes;
	int	fres;

	if (!parser)
		return ;
	pipe_count = count_cmd(parser) - 1;
	if (pipe_count < 0)
		pipe_count = 0;
	pipes = create_pipes(pipe_count);
	if (pipe_count > 0 && !pipes)
		return ;
	fres = forks_and_exec_commands(parser, pipes, pipe_count, main_struct);
	if (fres == 130)
	{
		close_all_pipes(pipes, pipe_count);
		main_struct->last_status = 130;
		return ;
	}
	close_all_pipes(pipes, pipe_count);
	wait_all_child(0, status, main_struct, parser);
}
