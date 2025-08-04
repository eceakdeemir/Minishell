#ifndef MINISHELL_H
# define MINISHELL_H

# include "built_in.h"
# include "enviroment.h"
# include "execute.h"
# include "parser.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/wait.h>

// free/free.c
void	free_argv(char **argv);

#endif