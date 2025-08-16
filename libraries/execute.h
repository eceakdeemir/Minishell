/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:19 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 18:11:12 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include <aio.h>

typedef struct s_wait_ctx
{
	int	saw_sigint;
	int	set_last;
}		t_wait_ctx;

int		execute(char **cmd, t_main_struct *main_struct, t_parser *parser);
void	check_signal_and_built(int control_value, t_main_struct *main_struct,
			t_parser *parser, char **cmd);
void	this_is_not_built_in(char **cmd, t_main_struct *main_struct,
			t_parser *parser);
void	main_run_built_in(t_main_struct *main_struct, t_parser *parser);
int		main_redirector(t_parser *parser, int control_value);

char	*check_is_path(char **argv, char *cmd);
char	*find_path(char *cmd, t_main_struct *main_struct);
void	env_converter_to_execve(t_main_struct *main_struct,
			char ***temporary_execve_env);

void	path_found_and_execute(t_parser *current, t_main_struct *main_struct);
void	arrangement_dup(int **pipes, int pipe_count, t_parser *current,
			int i);
void	execute_main(t_parser *parser, t_main_struct *main_struct);
void	wait_all_child(int i, int status, t_main_struct *main_struct,
			t_parser *parser);
int		forks_and_exec_commands(t_parser *parser, int **pipes,
			int pipe_count, t_main_struct *main_struct);

void	create_pipes(int pipe_count, int **pipes);
void	close_pipes(int pipe_count, int **pipes);
int		count_cmd(t_parser *parser);
void	is_there_signal_at_child(pid_t pid, int status,
			t_main_struct *main_struct, t_wait_ctx *ctx);

#endif