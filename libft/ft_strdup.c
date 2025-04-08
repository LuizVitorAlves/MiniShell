/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:40:07 by uviana-b          #+#    #+#             */
/*   Updated: 2024/09/17 15:17:13 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(char *src)
{
	char	*str_cpy;
	int		i;

	i = -1;
	str_cpy = (char *)malloc(ft_strlen(src) + 1 * sizeof(char));
	if (str_cpy == NULL)
		return (NULL);
	while (src[++i])
		str_cpy[i] = src[i];
	str_cpy[i] = '\0';
	return (str_cpy);
}
