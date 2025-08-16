/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:31:00 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 15:34:51 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

int	built_in_env(t_enviroment *env)
{
	while (env)
	{
		if (env->value && env->value[0] != '\0')
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}
