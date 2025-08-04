#include "../libraries/built_in.h"
#include "../libraries/minishell.h"

void	add_built_in_token(t_parser **head)
{
	t_parser	*new_parser;
	int			i;

	new_parser = *head;
	char *built_in[8] = {"echo", "cd", "pwd", "export", "unset", "env", "exit",
		NULL}; // onunda NULL var ki, döngüyü durdurmak için.
	while (new_parser)
	{
		i = 0;
		new_parser->built_type = -1;
		while (built_in[i])
		{
			if (ft_strcmp(new_parser->args[0], built_in[i]) == 0)
				new_parser->built_type = i;
			i++;
		}
		new_parser = new_parser->next;
	}
}

int	run_built_in(t_parser *parser, t_main_struct *main_struct)
{
	if (parser->built_type == ECHO)
		return (built_in_echo(parser));
	else if (parser->built_type == CD)
		return (built_in_cd(parser, main_struct->env_struct));
	else if (parser->built_type == PWD)
		return (built_in_pwd(parser));
	else if (parser->built_type == EXPORT)
		return (built_in_export(parser, main_struct->env_struct));
	else if (parser->built_type == UNSET)
		return (built_in_unset(parser, main_struct->env_struct));
	else if (parser->built_type == ENV)
		return (built_in_env(*main_struct->env_struct));
	else if (parser->built_type == EXIT)
		return (built_in_exit(parser));
	printf("DEBUG: no built-in matched, built_type=%d\n", parser->built_type);
	return (-1);
}