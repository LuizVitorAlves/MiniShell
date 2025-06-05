/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 22:45:37 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 01:24:14 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_double_char_ops(const char *line, int *i,
		t_token_type *type_out, char **op_str_out)
{
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		*type_out = TOKEN_HEREDOC;
		*op_str_out = "<<";
		*i += 2;
		return (1);
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		*type_out = TOKEN_APPEND;
		*op_str_out = ">>";
		*i += 2;
		return (1);
	}
	return (0);
}

static int	process_single_char_ops(const char *line, int *i,
		t_token_type *type_out, char **op_str_out)
{
	if (line[*i] == '<')
	{
		*type_out = TOKEN_REDIR_IN;
		*op_str_out = "<";
		*i += 1;
		return (1);
	}
	else if (line[*i] == '>')
	{
		*type_out = TOKEN_REDIR_OUT;
		*op_str_out = ">";
		*i += 1;
		return (1);
	}
	else if (line[*i] == '|')
	{
		*type_out = TOKEN_PIPE;
		*op_str_out = "|";
		*i += 1;
		return (1);
	}
	return (0);
}

static void	handle_operator(const char *line, int *i, t_token **head)
{
	char			*op_str;
	t_token_type	type;

	op_str = NULL;
	if (process_double_char_ops(line, i, &type, &op_str))
	{
	}
	else if (process_single_char_ops(line, i, &type, &op_str))
	{
	}
	if (op_str)
	{
		add_token(head, create_token(type, op_str));
	}
}

static int	process_token(const char *line, int *i, t_token **head)
{
	char	*word;

	if (ft_isspace(line[*i]))
		(*i)++;
	else if (ft_strchr("|<>", line[*i]))
		handle_operator(line, i, head);
	else
	{
		word = get_word(line, i);
		if (!word)
		{
			free_tokens(*head);
			*head = NULL;
			return (0);
		}
		if (*word)
			add_token(head, create_token(TOKEN_WORD, word));
		free(word);
	}
	return (1);
}

t_token	*tokenize(const char *line)
{
	t_token	*head;
	int		i;
	int		success;

	head = NULL;
	i = 0;
	success = 1;
	while (line[i] && success)
	{
		success = process_token(line, &i, &head);
	}
	if (!success)
		return (NULL);
	add_token(&head, create_token(TOKEN_EOF, ""));
	return (head);
}
