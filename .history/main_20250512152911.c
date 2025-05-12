#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


char **env_vars = NULL;

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
    char	*input;
	char	path_name[1024];
    t_token    *tokens;
	struct sigaction sa;
	struct sigaction sa_quit;

	sa.sa_handler = handler_ctr_c;
	sa_quit.sa_handler = SIG_IGN;
	(void )argv;
    (void )argc;
	create_env_arr(&env_vars);
	while(1)
    {
		ft_strlcpy(path_name, "minishell$", 11);
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa_quit, NULL);
        input = readline(path_name);
        if(!input)
            exit(0);
        else
			tokens = tokenize(input);
        if (input[0])
			add_history(input);
        
      
		// print_tokens(tokens);
		free_tokens(tokens);
        free(input);
    }
}
