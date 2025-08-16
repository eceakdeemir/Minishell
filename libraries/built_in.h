/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:12 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 16:42:45 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H
# include "enviroment.h"
# include "minishell.h"
# include "parser.h"

int		built_in_echo(t_parser *parser);

int		built_in_env(t_enviroment *env);

int		built_in_pwd(t_parser *parser);

int		built_in_exit(t_parser *parser, t_main_struct *main_struct);

int		built_in_cd(t_parser *parser, t_enviroment **env);

int		built_in_export(t_parser *parser, t_enviroment **env);

int		built_in_unset(t_parser *parser, t_enviroment **env);

void	add_built_in_token(t_parser **head);
int		run_built_in(t_parser *parser, t_main_struct *main_struct);

#endif