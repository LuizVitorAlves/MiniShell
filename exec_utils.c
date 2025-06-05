/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:38:30 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 17:41:44 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

static int	ft_isspace_atol(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

static int	ft_isdigit_atol(int c)
{
	return (c >= '0' && c <= '9');
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (ft_isspace_atol(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit_atol(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static size_t	ft_strlen_strndup(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static void	*ft_memcpy_strndup(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s = (const unsigned char *)src;
	size_t				i;

	d = (unsigned char *)dst;
	i = 0;
	if (!dst && !src)
		return (NULL);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*duplicate;
	size_t	len_s;
	size_t	copy_len;

	if (!s)
		return (NULL);
	len_s = ft_strlen_strndup(s);
	if (n < len_s)
		copy_len = n;
	else
		copy_len = len_s;
	duplicate = (char *)malloc(sizeof(char) * (copy_len + 1));
	if (!duplicate)
		return (NULL);
	ft_memcpy_strndup(duplicate, s, copy_len);
	duplicate[copy_len] = '\0';
	return (duplicate);
}
