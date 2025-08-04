
#ifndef EXECUTE_H
# define EXECUTE_H

# include <aio.h>

// execute_funcitons.c
int		count_cmd(t_parser *parser);
void	execute(char **cmd, t_main_struct *main_struct, t_parser *parser);

// execute_path.c
char	*check_is_path(char **argv, char *cmd);
char	*find_path(char *cmd, char **env);
void	this_is_not_built_in(char **cmd, t_main_struct *main_struct,
			t_parser *parser);

// open_pipe_fork.c
void	create_pipes(int pipe_count, int pipes[][2]);
void	close_pipes(int pipe_count, int pipes[][2]);
void	path_found_and_execute(t_parser *current, char **env);
void	wait_all_child_process(t_parser *parser);
void	forks_and_exec_commands(t_parser *parser, int pipes[][2],
			int pipe_count, char **env);
void	execute_main(t_parser *parser, char **env);

#endif