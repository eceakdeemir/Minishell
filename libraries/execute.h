/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:19 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/19 20:31:02 by igurses          ###   ########.fr       */
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

// void	create_pipes(int pipe_count, int **pipes);
// void	close_pipes(int pipe_count, int **pipes);
void	arrangement_dup(int **pipes, int pipe_count, t_parser *current, int i);
int		forks_and_exec_commands(t_parser *parser, int **pipes, int pipe_count,
			t_main_struct *main_struct);

int		main_redirector(t_parser *parser, int mode);

int		count_cmd(t_parser *parser);
void	wait_all_child(int i, int status, t_main_struct *main_struct,
			t_parser *parser);
void	is_there_signal_at_child(pid_t pid, int status,
			t_main_struct *main_struct, t_wait_ctx *ctx);

#endif