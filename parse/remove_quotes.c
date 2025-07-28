#include "../libraries/minishell.h"

char *remove_quotes(char *word) // 25'den fazla satır uzunluğı  
{
	char *result;
	int i;
	int j;

	result = malloc(ft_strlen(word) + 1);
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
	return(result);
}

void remove_quotes_all(t_lexer **head)
{
	t_lexer *temp;
	char *new_word;

	temp = *head;
	while (temp != NULL)
	{
		if (temp->token_enum == TOKEN_WORD)
		{
			new_word = remove_quotes(temp->word);
			free(temp->word);
			temp->word = new_word;
		}
		temp = temp->next;
	}
}
