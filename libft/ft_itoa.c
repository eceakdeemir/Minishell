/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 18:01:09 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/21 02:22:32 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nums_len(int n)
{
	int	len;

	len = 0;
	if (n < 0)
		len++;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	long	number;
	char	*result;

	number = n;
	len = ft_nums_len(n);
	result = (char *)memory_malloc(sizeof(char) * len + 1);
	if (!result)
		return (NULL);
	if (number == 0)
		result[0] = '0';
	result[len] = '\0';
	if (number < 0)
	{
		result[0] = '-';
		number *= -1;
	}
	while (number > 0)
	{
		result[--len] = (number % 10) + '0';
		number = number / 10;
	}
	return (result);
}
