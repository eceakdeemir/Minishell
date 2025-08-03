#ifndef MINISHELL_H
# define MINISHELL_H

# include "parser.h"
# include "enviroment.h"
# include "execute.h"
#include "built_in.h"
# include <readline/readline.h>
# include <readline/history.h>
#include <sys/wait.h>

//free/free.c
void free_argv(char **argv);

#endif