/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igurses <igurses@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:33 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 18:38:16 by igurses          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "built_in.h"
# include "enviroment.h"
# include "execute.h"
# include "parser.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include "memory.h"

# define INTERACTIVE 0
# define EXECUTING 1
# define HEREDOC 2

extern sig_atomic_t	g_signal;

int					redirector_funct(t_parser *parser);

int					append(t_redirector *redirector);

int					output(t_redirector *redirector_tmp);

int					input(t_redirector *redirector);

int					prepare_heredocs(t_parser *parser, t_enviroment *env,
						t_main_struct *main_struct);

void				setup_signals(int context);
void				signals_interactive(void);
void				signals_executing(void);
void				signals_heredoc(void);
void				reset_signals(void);
void				sigint_interactive(int signo);
void				handle_sigint_executing(int signo);
void				handle_sigquit_executing(int signo);
void				sigint_heredoc(int signo);
void				set_sig(int sig, void (*h)(int));
void				enter_heredoc_parent_mode(void);
void				restore_interactive_mode(void);

void				compact_args(char **args);
void				compact_all_commands(t_parser *head);
void				message_error2(const char *name, const char *msg);
int					has_slash(const char *s);

char				*control_path(char **cmd, t_main_struct *main_struct);
void				is_acces_path(char *path);
void				exec_or_die(const char *path, char **argv,
						t_main_struct *main_struct);
void				heredoc_tokenize_expender(char **line,
						t_main_struct *main_struct);

#endif
