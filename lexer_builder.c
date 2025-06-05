/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 22:51:15 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 18:00:10 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builder_init(t_builder *b)
{
	b->capacity = 32;
	b->len = 0;
	b->str = malloc(sizeof(char) * b->capacity);
	if (b->str)
		b->str[0] = '\0';
}

void	builder_destroy(t_builder *b)
{
	if (b->str)
	{
		free(b->str);
		b->str = NULL;
	}
	b->len = 0;
	b->capacity = 0;
}

void	builder_append_char(t_builder *b, char c)
{
	void	*new_ptr;

	if (!b->str)
		return ;
	if (b->len + 1 >= b->capacity)
	{
		b->capacity *= 2;
		new_ptr = malloc(b->capacity);
		if (!new_ptr)
			return ;
		ft_memcpy(new_ptr, b->str, b->len);
		free(b->str);
		b->str = new_ptr;
	}
	b->str[b->len++] = c;
	b->str[b->len] = '\0';
}

void	builder_append_str(t_builder *b, char *str)
{
	if (!str)
		return ;
	while (*str)
		builder_append_char(b, *str++);
}

char	*builder_finalize(t_builder *b)
{
	char	*final_str;

	if (!b->str)
		return (NULL);
	final_str = ft_strdup(b->str);
	builder_destroy(b);
	return (final_str);
}
