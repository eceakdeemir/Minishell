/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:31:19 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/14 14:23:26 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

t_enviroment *create_new_node_for_env(char *key, char *value) // enviroment için yeni node açar
{
    t_enviroment *env;

    env = mem_malloc(sizeof(t_enviroment));
    if (!env)
        return(NULL);
    env->key = key;
    env->value = value;
    env->next = NULL;
    return (env);
}

void add_new_node_for_env(t_enviroment **head, t_enviroment *env) // enviroment için list oluşturur 
{
    t_enviroment *temp;
    
    if (!(*head))
    {
        *head = env;
        return ;
    }
    temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = env;
}

char *extract_key(char *str) //  sadece PATH ksımını almak için 
{
    int i;
    
    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    return ft_strndup(str, i);
}

char *extract_value(char *str) // sadece usr/bin gibi değerleri almak için
{
    char *equal;

    equal = ft_strchr(str, '=');
    if (!equal)
        return ft_strdup("");
    return ft_strdup(equal + 1);
}


char *get_env_value(const char *key, t_enviroment *env)//env listesinden belirli bir keyi bulup value'sunu döndürür. sadece okur
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (ft_strdup(env->value));
        env = env->next;
    }
    return (NULL);
}

void update_env_value(const char *key, const char *value, t_enviroment **env)//verdiğim key'in value değerini değiştirir.
{
    t_enviroment *tmp;

    tmp = *env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            tmp->value = ft_strdup(value);
            return ;
        }
        tmp = tmp->next;
    }
    add_new_node_for_env(env, create_new_node_for_env(ft_strdup(key), ft_strdup(value)));
}

void init_env(char **envp, t_enviroment **env_list)
{
    t_enviroment *new_node;

    char *key;
    char *value;
    int i;

    i = 0;
    while (envp[i])
    {
        key = extract_key(envp[i]);
        value = extract_value(envp[i]);
        new_node = create_new_node_for_env(key, value);
        add_new_node_for_env(env_list, new_node);
        i++;
    }
}