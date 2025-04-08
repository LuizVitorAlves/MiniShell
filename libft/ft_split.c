/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:31:22 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/22 14:04:26 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	ft_count_words(const char *s, char c)
{
	size_t	count;
	int		inword;
	int		i;

	count = 0;
	inword = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && inword == 0)
		{
			count++;
			inword = 1;
		}
		else if (s[i] == c)
			inword = 0;
		i++;
	}
	return (count);
}

static int	free_str(char **strs)
{
	int		i;

	i = 0;
	while (strs[i] != 0)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (0);
}

static int	ft_populate(char **strs, char *s, char c)
{
	char	*start;
	int		len;
	int		i;

	start = s;
	len = 1;
	i = 0;
	while (*s)
	{
		if (*s == c && *(s + 1) != c)
		{
			start = (char *)(s + 1);
			len = 0;
		}
		if (*s != c && (*(s + 1) == c || *(s + 1) == 0))
		{
			strs[i] = ft_substr(start, 0, len);
			if (strs[i++] == 0)
				return (free_str(strs));
		}
		len++;
		s++;
	}
	strs[i] = 0;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	char	**ret;

	if (!s)
		return (NULL);
	ret = (char **) malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (ret == NULL)
		return (NULL);
	ret[ft_count_words(s, c)] = NULL;
	ft_populate(ret, (char *)s, c);
	return (ret);
}
