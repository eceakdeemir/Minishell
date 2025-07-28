#include "../libraries/parser.h"

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

void lexer_print(t_lexer *list)
{
    while (list)
    {
        printf("%d %s \n", list->token_enum, list->word);
        list = list->next;
    }
}