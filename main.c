#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
/*
const char *get_token_name(t_token_type type)
{
	switch (type)
	{
		case TOKEN_WORD: return "WORD";
		case TOKEN_PIPE: return "PIPE";
		case TOKEN_REDIR_IN: return "REDIR_IN";
		case TOKEN_REDIR_OUT: return "REDIR_OUT";
		case TOKEN_APPEND: return "APPEND";
		case TOKEN_HEREDOC: return "HEREDOC";
		case TOKEN_EOF: return "EOF";
		default: return "UNKNOWN";
	}
}

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_token	*tmp;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break;
		if (*line)
			add_history(line);
		tokens = lexer(line);
		tmp = tokens;
		while (tmp)
		{
			printf("type: %-10s | value: '%s'\n", get_token_name(tmp->type), tmp->value);
			tmp = tmp->next;
		}
		while (tokens)
		{
			tmp = tokens->next;
			free(tokens->value);
			free(tokens);
			tokens = tmp;
		}
		free(line);
	}
	return (0);
}
*/

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
			printf("WORD: '%s'\n", tokens->value);
		else if (tokens->type == TOKEN_PIPE)
			printf("PIPE: '%s'\n", tokens->value);
		else if (tokens->type == TOKEN_REDIR_IN)
			printf("REDIR_IN: '%s'\n", tokens->value);
		else if (tokens->type == TOKEN_REDIR_OUT)
			printf("REDIR_OUT: '%s'\n", tokens->value);
		else if (tokens->type == TOKEN_APPEND)
			printf("APPEND: '%s'\n", tokens->value);
		else if (tokens->type == TOKEN_HEREDOC)
			printf("HEREDOC: '%s'\n", tokens->value);
		else if (tokens->type == TOKEN_EOF)
			printf("EOF\n");
		tokens = tokens->next;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

int	main(void)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		if (line[0])
			add_history(line);
		if (strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		tokens = tokenize(line);
		print_tokens(tokens);
		free_tokens(tokens);
		free(line);
	}
	return (0);
}
