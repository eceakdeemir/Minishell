/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:19 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 18:13:47 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"

typedef struct s_wait_ctx
{
	int	saw_sigint;
	int	set_last;
}		t_wait_ctx;

void	execute_main(t_parser *parser, t_main_struct *main_struct);
void	path_found_and_execute(t_parser *current, t_main_struct *main_struct);
void	prepare_execute(char **cmd, t_main_struct *main_struct,
			t_parser *parser);
void	this_is_not_built_in(char **cmd, t_main_struct *main_struct,
			t_parser *parser);

// Path ve environment işlemleri
char	*find_path(char *cmd, t_main_struct *main_struct);
void	env_converter_to_execve(t_main_struct *main_struct, char ***env);

void	arrangement_dup(int **pipes, int pipe_count, t_parser *current, int i);
int		forks_and_exec_commands(t_parser *parser, int **pipes, int pipe_count,
			t_main_struct *main_struct);

int		main_redirector(t_parser *parser, int mode);

int		count_cmd(t_parser *parser);
void	wait_all_child(int i, int status, t_main_struct *main_struct,
			t_parser *parser);
void	is_there_signal_at_child(pid_t pid, int status,
			t_main_struct *main_struct, t_wait_ctx *ctx);
void	close_all_pipes(int **pipes, int pipe_count);
int		**create_pipes(int pipe_count);
void	handle_wait_result(pid_t pid, int *i);
int		heredoc_count(t_parser *parse_node);
void	close_heredoc_fds(t_parser *current);

void	process_heredoc_line(char **line, int hd_no_expand,
			t_main_struct *main_struct);
int		open_one_heredoc(t_redirector *r, t_main_struct *main_struct);
void	main_heredoc_child_process(char *limiter, t_main_struct *main_struct,
			int hd_no_expand, int pipefd);
int		prepare_all_heredocs(t_parser *parser, t_main_struct *main_struct);

int		if_no_heredoc(t_redirector *parser);
void	finalize_wait_status(t_wait_ctx *ctx, t_main_struct *main_struct);
void	this_is_not_built_in(char **cmd, t_main_struct *main_struct,
			t_parser *parser);

void	this_is_not_built_in_fork(char **cmd, t_main_struct *main_struct,
			t_parser *parser, pid_t pid);

int		prep_control(t_parser *parser, t_main_struct *main_struct);
void	close_fd_and_child_control(t_parser *current, pid_t pid,
			t_main_struct *main_struct);
int		open_one_heredoc_child(pid_t pid, t_redirector *r,
			t_main_struct *main_struct, int fd[2]);

int		open_one_heredoc_parent(pid_t pid, int fd[2],
			t_main_struct *main_struct, int status);

void	write_for_main_heredoc_child_process(int pipefd, char *line);
#endif