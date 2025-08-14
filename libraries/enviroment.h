/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:15 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/14 14:23:30 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
#define ENV_H


typedef struct s_enviroment
{
    char *key;
    char *value;
    struct s_enviroment *next;
} t_enviroment;


t_enviroment *create_new_node_for_env(char *key, char *value);
void add_new_node_for_env(t_enviroment **head, t_enviroment *env);
char *get_env_value(const char *key, t_enviroment *env);
void update_env_value(const char *key, const char *value, t_enviroment **env);
void init_env(char **envp, t_enviroment **env_list);
char *extract_key(char *str);
char *extract_value(char *str);
#endif