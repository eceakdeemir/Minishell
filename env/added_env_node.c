/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   added_env_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:54:34 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 02:22:32 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

t_enviroment	*create_new_node_for_env(char *key, char *value)
{
	t_enviroment	*env;

	env = memory_malloc(sizeof(t_enviroment));
	if (!env)
		return (NULL);
	env->key = key;
	env->value = value;
	env->next = NULL;
	return (env);
}

void	add_new_node_for_env(t_enviroment **head, t_enviroment *env)
{
	t_enviroment	*temp;

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
