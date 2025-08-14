/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrahimberatgurses <ibrahimberatgurses@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:12 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/14 15:07:04 by ibrahimbera      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BUILT_IN_H
# define BUILT_IN_H
# include "minishell.h"

//built_in_echo.c
int built_in_echo(t_parser *parser);

// built_in_env.c
int built_in_env(t_enviroment *env);

// built_in_pwd.c
int built_in_pwd(t_parser *parser);

//built_in_exit.c
int built_in_exit(t_parser *parser, t_main_struct *main_struct);

int built_in_cd(t_parser *parser, t_enviroment **env);

int built_in_export(t_parser *parser, t_enviroment **env);

int built_in_unset(t_parser *parser, t_enviroment **env);

void add_built_in_token(t_parser **head);
int run_built_in(t_parser *parser, t_main_struct *main_struct);



#endif