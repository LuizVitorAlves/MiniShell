/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:30:16 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/21 08:30:18 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	size_t	len;

	if (!s1)
		return (0);
	i = 0;
	while (s1[i] && ft_strchr(set, *(s1 + i)) != 0)
		i++;
	if (s1[i] == 0)
		len = 1;
	else
		len = ft_strlen(s1 + i);
	while (len && ft_strchr(set, s1[len + i]) != 0)
		len--;
	return (ft_substr(s1 + i, 0, len + 1));
}
