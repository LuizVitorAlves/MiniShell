/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:18:52 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/22 14:06:16 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char		*t1;
	unsigned char		*t2;
	int					i;

	if (n == 0)
		return (0);
	t1 = ((unsigned char *) s1);
	t2 = ((unsigned char *) s2);
	i = 0;
	while (n > 0)
	{
		if (t1[i] != t2[i])
		{
			return (t1[i] - t2[i]);
		}
		n--;
		i++;
	}
	return (0);
}
