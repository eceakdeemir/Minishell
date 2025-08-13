/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:33 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/13 18:04:03 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libmem/incs/mem_utils.h"
#include "parser.h"
#include "enviroment.h"
#include "execute.h"
#include "built_in.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define INTERACTIVE_MODE 0
#define EXECUTING_MODE 1
#define HEREDOC_MODE 2
extern sig_atomic_t g_signal;

// free/free.c
void free_argv(char **argv);

// redirector/redirector.c
int redirector_funct(t_parser *parser);

// redirector/append.c
int append(t_redirector *redirector);

// redirector/output.c
int output(t_redirector *redirector);

// redirector/input.c
int input(t_redirector *redirector);

// redirector/herodoc.c
int prepare_heredocs(t_parser *parser, t_enviroment *env, t_main_struct *main_struct);

// signal
void setup_signals(int context);
void setup_signals_interactive(void);
void setup_signals_executing(void);
void setup_signals_heredoc(void);
void reset_signals(void);
void handle_sigint_interactive(int signo);
void handle_sigint_executing(int signo);
void handle_sigquit_executing(int signo);
void handle_sigint_heredoc(int signo);

// utils/helper_utils.c
void compact_args(char **args);
void compact_all_commands(t_parser *head);
void message_error2(const char *name, const char *msg);
int has_slash(const char *s);

// utils/helper_for_execute.c
char *control_path(char **cmd, t_main_struct *main_struct);
void is_acces_path(char *path);
void	exec_or_die(const char *path, char **argv, t_main_struct *main_struct);

#endif
