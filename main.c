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

// int	main(void)
// {
// 	char	*line;
// 	t_token	*tokens;

// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (!line)
// 			break ;
// 		if (line[0])
// 			add_history(line);
// 		if (strcmp(line, "exit") == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		tokens = tokenize(line);
// 		print_tokens(tokens);
// 		free_tokens(tokens);
// 		free(line);
// 	}
// 	return (0);
// }
void	handler_sa_quit(int sig)
{
	(void) sig;
	rl_replace_line("", 0);     // limpa o conteúdo da linha atual
    write(STDOUT_FILENO, "", 1); // pular linha        // informa que estamos em uma nova linha
    rl_redisplay();   
}
void	handler_ctr_c(int sig)
{
	(void) sig;

	rl_replace_line("", 0);     // limpa o conteúdo da linha atual
    write(STDOUT_FILENO, "\n", 1); // pular linha
    rl_on_new_line();           // informa que estamos em uma nova linha
    rl_redisplay();             // reexibe o prompt
}

int main(int argc, char *argv[])
{
    char *input;
    char    **args;
	struct sigaction sa;
	struct sigaction sa_quit;

	sa.sa_handler = handler_ctr_c;
	sa_quit.sa_handler = SIG_IGN;
	(void )argv;
    (void )argc;
    while(1)
    {
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa_quit, NULL);
        input = readline("minishell$");
        if(!input)
        {
            exit(0);
        }
        else
        {
            args = put_args_array(input);
        }
        if (input[0])
			add_history(input);
        
        if(ft_strncmp(args[0], "exit", 4) == 0 && (args[0][4] == '\0' || args[0][4] == ' '))
        {
            //liberar a matriz args
            //liberar a matriz input
            exit(0);
        }
        // tokens = tokenize(input);
		// print_tokens(tokens);
		// free_tokens(input);
        free(input);
    }
}