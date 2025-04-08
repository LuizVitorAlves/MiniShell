/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:28:37 by uviana-b          #+#    #+#             */
/*   Updated: 2024/09/26 11:51:20 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;
	size_t	count;

	i = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	count = dst_len;
	if (size <= dst_len)
		return (size + src_len);
	while (src[i] != 0 && count +1 < size)
	{
		dst[count] = src[i];
		i++;
		count++;
	}
	dst[count] = '\0';
	return (src_len + dst_len);
}
