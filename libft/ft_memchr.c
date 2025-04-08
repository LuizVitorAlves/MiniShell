/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:57:50 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/22 14:00:16 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char		*s2;
	unsigned char		c2;
	int					i;

	if (c == 0 && n == 0)
		return (0);
	s2 = ((unsigned char *)s);
	c2 = ((unsigned char)c);
	i = 0;
	while (n--)
	{
		if (s2[i] == c2)
			return (s2 + i);
		i++;
	}
	return (NULL);
}
