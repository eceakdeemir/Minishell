/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:31:14 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/19 15:40:56 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	control_new_parser_args(t_parser *new_parser)
{
	if (!new_parser->args || !new_parser->args[0])
	{
		new_parser = new_parser->next;
		return (-1);
	}
	return (0);
}

void	add_built_in_token(t_parser **head)
{
	t_parser	*new_parser;
	int			i;
	char const	*built_in[8] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit", NULL};

	new_parser = *head;
	while (new_parser)
	{
		new_parser->built_type = -1;
		if (control_new_parser_args(new_parser) == -1)
			break ;
		i = 0;
		while (built_in[i])
		{
			if (ft_strcmp(new_parser->args[0], built_in[i]) == 0)
			{
				new_parser->built_type = i;
				break ;
			}
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
		return (built_in_exit(parser, main_struct));
	return (-1);
}
