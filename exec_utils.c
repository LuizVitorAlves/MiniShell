/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:38:30 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/14 19:51:48 by uviana-b         ###   ########.fr       */
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
