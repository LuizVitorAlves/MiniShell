/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 01:22:51 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 01:24:31 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_word(const char *line, int *i)
{
	t_builder	builder;
	int			success;

	builder_init(&builder);
	success = 1;
	while (success && line[*i] && !ft_isspace(line[*i]) && !ft_strchr("|<>",
			line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '\"')
			success = get_quoted_part(line, i, &builder);
		else
			get_unquoted_part(line, i, &builder);
	}
	if (!success)
	{
		builder_destroy(&builder);
		exit_status(1);
		return (NULL);
	}
	return (builder_finalize(&builder));
}
