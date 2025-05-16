/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:35:19 by lalves-d          #+#    #+#             */
/*   Updated: 2025/05/16 15:57:00 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = strdup(value);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*temp;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
}

char	*get_quoted_word(const char *line, int *i, char quote)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	len = 0;
	while (line[*i] && line[*i] != quote)
	{
		len++;
		(*i)++;
	}
	if (line[*i] != quote)
	{
		write(2, "syntax error: unclosed quote\n", 29);
		return (NULL);
	}
	word = strndup(&line[start], len);
	(*i)++;
	return (word);
}


char	*get_unquoted_word(const char *line, int *i)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	len = 0;
	while (line[*i]
		&& !isspace(line[*i])
		&& line[*i] != '|'
		&& line[*i] != '<'
		&& line[*i] != '>')
	{
		len++;
		(*i)++;
	}
	word = strndup(&line[start], len);
	return (word);
}

char	*get_word(const char *line, int *i)
{
	char	*word;

	while (line[*i] && isspace(line[*i]))
		(*i)++;
	if (line[*i] == '\'' || line[*i] == '"')
	{
		(*i)++;
		word = get_quoted_word(line, i, line[*i - 1]);
		if (!word)
			return (NULL);
	}
	else
		word = get_unquoted_word(line, i);
	return (word);
}

char	*get_op_str(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return ("|");
	else if (type == TOKEN_REDIR_IN)
		return ("<");
	else if (type == TOKEN_REDIR_OUT)
		return (">");
	else if (type == TOKEN_APPEND)
		return (">>");
	else if (type == TOKEN_HEREDOC)
		return ("<<");
	return ("");
}

t_token_type	get_token_type(const char *line, int *i)
{
	if (line[*i] == '|')
	{
		(*i)++;
		return (TOKEN_PIPE);
	}
	else if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			*i += 2;
			return (TOKEN_HEREDOC);
		}
		(*i)++;
		return (TOKEN_REDIR_IN);
	}
	else if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			*i += 2;
			return (TOKEN_APPEND);
		}
		(*i)++;
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD);
}

t_token	*tokenize(const char *line)
{
	t_token			*head;
	t_token			*new_token;
	t_token_type	type;
	char			*word;
	int				i;

	head = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		{
			type = get_token_type(line, &i);
			new_token = create_token(type, get_op_str(type));
			add_token(&head, new_token);
		}
		else
		{
			word = get_word(line, &i);
			if (!word)
			{
				return (head);
			}
			new_token = create_token(TOKEN_WORD, word);
			free(word);
			add_token(&head, new_token);
		}
	}
	add_token(&head, create_token(TOKEN_EOF, ""));
	return (head);
}
