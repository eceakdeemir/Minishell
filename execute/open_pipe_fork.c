/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_pipe_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrahim <ibrahim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:32:37 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/19 11:54:47 by ibrahim          ###   ########.fr       */
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
	reset_signals();
	// STDIN yönetimi
	if (current->redirector && !if_no_heredoc(current->redirector))
	{
		// Heredoc varsa, STDIN'i heredoc'a yönlendir
		dup2(current->redirector->herodoc_fd, STDIN_FILENO);
	}
	else if (i > 0)
	{
		// İlk komut değilse, önceki pipe'dan oku
		dup2(pipes[i - 1][0], STDIN_FILENO);
	}
	// STDOUT yönetimi
	if (i < pipe_count)
	{
		// Son komut değilse, sonraki pipe'a yaz
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	// Kullanılmayan pipe'ları kapat
	for (int j = 0; j < pipe_count; j++)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
	}
	// Heredoc fd'yi kapat
	if (current->redirector && !if_no_heredoc(current->redirector))
	{
		close(current->redirector->herodoc_fd);
	}
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

static int	open_one_heredoc(t_redirector *r, t_main_struct *main_struct)
{
	int		fd[2];
	pid_t	pid;
	int		status;
	char	*line;
	int		delimiter_hit;

	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	enter_heredoc_parent_mode();
	pid = fork();
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
		/* CHILD */
		set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_IGN);
		close(fd[0]);
		delimiter_hit = 0;
		while (1)
		{
			line = readline("> ");
			if (!line) /* Ctrl+D (EOF) */
			{
				/* Bash benzeri: newline yoksa ekle */
				write(STDOUT_FILENO, "\n", 1);
				break ;
			}
			if (ft_strcmp(line, r->file) == 0)
			{
				delimiter_hit = 1;
				free(line);
				break ;
			}
			if (!r->hd_no_expand)
				line = heredoc_tokenize_expender(line,
						*(main_struct->env_struct), main_struct);
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);
		}
		close(fd[1]);
		ft_exit(0);
	}
	/* PARENT */
	close(fd[1]);
	if (waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid");
		close(fd[0]);
		restore_interactive_mode();
		return (-1);
	}
	restore_interactive_mode();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1); /* Ctrl+C heredoc: prompt hizalansın */
		close(fd[0]);
		main_struct->last_status = 130;
		g_signal = SIGINT;
		return (-2);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(fd[0]);
		return (-1);
	}
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
				if (fd == -2) /* SIGINT */
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

int	forks_and_exec_commands(t_parser *parser, int **pipes, int pipe_count,
		t_main_struct *main_struct)
{
	t_parser		*current;
	pid_t			pid;
	int				i;
	int				control_redirector;
	t_redirector	*r;
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
		{
			arrangement_dup(pipes, pipe_count, current, i);
			if (current->redirector)
			{
				control_redirector = main_redirector(current, 0);
				if (control_redirector == -1)
					ft_exit(1);
			}
			if (current->built_type >= 0 && current->built_type <= 6)
				ft_exit(run_built_in(current, main_struct));
			else
				path_found_and_execute(current, main_struct);
		}
		else if (pid < 0)
		{
			perror("fork");
			return (1);
		}
		/* Parent heredoc fd kapat */
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
		if (!current->next)
			main_struct->last_child_pid = pid;
		current = current->next;
		i++;
	}
	return (0);
}

void	wait_all_child(int i, int status, t_main_struct *main_struct,
		t_parser *parser)
{
	pid_t		pid;
	t_wait_ctx	ctx;
	int			saw_sigint_newline;

	ctx.saw_sigint = 0;
	ctx.set_last = 0;
	saw_sigint_newline = 0;
	i = count_cmd(parser);
	while (i > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid > 0)
		{
			i--;
			is_there_signal_at_child(pid, status, main_struct, &ctx);
		}
		else if (pid < 0)
		{
			if (errno == EINTR)
				continue;   
			if (errno == ECHILD)
				break ;
			perror("waitpid");
			break ;
		}
	}
	if (ctx.saw_sigint)
	{
		write(STDOUT_FILENO, "\n", 1);
		saw_sigint_newline = 1;
	}
	if (ctx.saw_sigint && !ctx.set_last)
		main_struct->last_status = 130;
	g_signal = 0;
	(void)saw_sigint_newline;
}

void	execute_main(t_parser *parser, t_main_struct *main_struct)
{
	int	pipe_count;
	int	status;
	int	i;
	int	**pipes;
	int	fres;

	if (!parser)
		return ;
	pipe_count = count_cmd(parser) - 1;
	if (pipe_count < 0)
		pipe_count = 0;
	pipes = NULL;
	if (pipe_count > 0)
	{
		pipes = mem_malloc(sizeof(int *) * pipe_count);
		for (i = 0; i < pipe_count; i++)
		{
			pipes[i] = mem_malloc(sizeof(int) * 2);
			if (pipe(pipes[i]) == -1)
				return (perror("pipe"));
		}
	}
	fres = forks_and_exec_commands(parser, pipes, pipe_count, main_struct);
	if (fres == 130)
	{
		if (pipes)
		{
			for (i = 0; i < pipe_count; i++)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
			}
		}
		main_struct->last_status = 130;
		return ;
	}
	if (pipes)
	{
		for (i = 0; i < pipe_count; i++)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
		}
	}
	wait_all_child(0, status, main_struct, parser);
}
