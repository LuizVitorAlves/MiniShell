#include "minishell.h"

static void	handle_operator(const char *line, int *i, t_token **head)
{
	char			*op_str;
	t_token_type	type;

	op_str = NULL;
	if (line[*i] == '<' && line[*i + 1] == '<')
		{type = TOKEN_HEREDOC; op_str = "<<"; *i += 2;}
	else if (line[*i] == '>' && line[*i + 1] == '>')
		{type = TOKEN_APPEND; op_str = ">>"; *i += 2;}
	else if (line[*i] == '<')
		{type = TOKEN_REDIR_IN; op_str = "<"; *i += 1;}
	else if (line[*i] == '>')
		{type = TOKEN_REDIR_OUT; op_str = ">"; *i += 1;}
	else if (line[*i] == '|')
		{type = TOKEN_PIPE; op_str = "|"; *i += 1;}
	if (op_str)
		add_token(head, create_token(type, op_str));
}

static char	*get_word(const char *line, int *i)
{
	t_builder	builder;
	int			success;

	builder_init(&builder);
	success = 1;
	while (success && line[*i] && !ft_isspace(line[*i]) \
		&& !ft_strchr("|<>", line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '\"')
			success = get_quoted_part(line, i, &builder);
		else
			get_unquoted_part(line, i, &builder);
	}
	if (!success)
	{
		builder_destroy(&builder);
		return (NULL);
	}
	return (builder_finalize(&builder));
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
