#ifndef MINISHELL_H
#define MINISHELL_H

#include "parser.h"
#include "enviroment.h"


typedef struct s_main_struct
{
	t_enviroment **env_struct;
	char **env;

} t_main_struct;




#endif