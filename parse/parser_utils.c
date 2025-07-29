#include "../libraries/minishell.h"

int ft_isspace(char line)
{
    if (line == ' ' || line == '\t' || line == '\n' || line == '\v' || line == '\r' || line == '\f')
        return (1);
    return (0);
}

char *ft_strndup(const char *src, int len)
{
    char *dest = malloc(len + 1);
    int i;

    if (!dest)
        return NULL;
    i = 0;
    while (i < len)
    {
        dest[i] = src[i];
        i++;
    }
    dest[len] = '\0';
    return dest;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((str1[i] || str2[i]))
	{
		if (str1[i] > str2[i])
			return (1);
		else if (str1[i] < str2[i])
			return (-1);
		i++;
	}
	return (0);
}

int count_args(t_lexer *lexer)
{
	int count;
	t_lexer *temp;

	temp = lexer;
	if (!temp)
		return (0);
	count = 0;
	while (temp && temp->token_enum != TOKEN_PIPE)
	{
		// printf("seg yedim\n");
		if (temp->token_enum == TOKEN_WORD)
		{
			count++;
 			temp = temp->next; 
		}
		else
		{
            temp = temp->next; // hedefe geç
            if (temp)
                temp = temp->next; // hedefi de geç
		}
	}
	return(count);
}