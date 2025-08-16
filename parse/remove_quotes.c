/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:04 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 16:49:32 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*remove_quotes(char *word)
{
	char	*result;
	int		i;
	int		j;

	result = mem_malloc(ft_strlen(word) + 1);
	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] == '\'')
		{
			i++;
			while (word[i] && word[i] != '\'')
				result[j++] = word[i++];
			if (word[i] == '\'')
				i++;
		}
		else if (word[i] == '\"')
		{
			i++;
			while (word[i] && word[i] != '\"')
				result[j++] = word[i++];
			if (word[i] == '\"')
				i++;
		}
		else
			result[j++] = word[i++];
	}
	result[j] = '\0';
	return (result);
}

void	remove_quotes_all(t_lexer **head)
{
	t_lexer	*temp;
	char	*new_word;

	temp = *head;
	while (temp != NULL)
	{
		if (temp->token_enum == TOKEN_WORD)
		{
			new_word = remove_quotes(temp->word);
			temp->word = new_word;
		}
		temp = temp->next;
	}
}
