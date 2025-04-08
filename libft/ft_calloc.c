/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:45:42 by uviana-b          #+#    #+#             */
/*   Updated: 2024/09/26 16:10:29 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*ret;
	size_t	cont;

	if (num == 0 || size == 0)
	{
		cont = 1;
	}
	else
	{
		cont = size * num;
		if (cont / num != size)
			return (NULL);
	}
	ret = (void *) malloc(cont);
	if (!ret)
		return (NULL);
	ft_bzero(ret, cont);
	return (ret);
}
