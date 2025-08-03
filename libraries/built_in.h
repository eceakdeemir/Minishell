
#ifndef BUILT_IN_H
#define BUILT_IN_H

#include "minishell.h"

//built_in_echo.c
int built_in_echo(t_parser *parser);

// built_in_env.c
int built_in_env(t_enviroment *env);

// built_in_pwd.c
int built_in_pwd(t_parser *parser);

//built_in_exit.c
int built_in_exit(t_parser *parser);



#endif