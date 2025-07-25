/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 15:30:54 by ecakdemi          #+#    #+#             */
/*   Updated: 2024/10/30 16:49:09 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	del(char **str, int strc)
{
	int	i;

	i = 0;
	while (i < strc)
	{
		free(str[i]);
		i++;
	}
	free (str);
	return (1);
}

static int	ft_string_count(char const *s, char c)
{
	int	i;
	int	string_count;

	i = 0;
	string_count = 0;
	while (s[i])
	{
		if (s[i] == c && i > 0 && s[i - 1] != c)
		{
			while (s[i + 1] != '\0' && s[i] == c)
				i++;
			string_count++;
		}
		i++;
	}
	if (s[i - 1] != c && i > 0)
		string_count++;
	return (string_count);
}

static int	ft_string_appender(char const *s, char **str, char c,
			int string_count)
{
	int	i;
	int	strc;
	int	start;

	i = 0;
	strc = 0;
	start = 0;
	while (strc < string_count)
	{
		if (s[i] != c && i > 0 && s[i - 1] == c)
			start = i;
		if ((s[i] == c && i > 0 && s[i - 1] != c) || s[i] == '\0')
		{
			str[strc] = ft_substr(s, start, i - start);
			if (!str[strc])
				return (del(str, strc));
			while (s[i] && s[i + 1] != '\0' && s[i + 1] == c)
				i++;
			strc++;
		}
		i++;
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		string_count;

	string_count = ft_string_count(s, c);
	str = (char **)malloc(sizeof(char *) * (string_count + 1));
	if (!str)
		return (NULL);
	str[string_count] = NULL;
	if (ft_string_appender(s, str, c, string_count) == 1)
		return (NULL);
	return (str);
}
