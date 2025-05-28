#define _DEFAULT_SOURCE //Verificar na 42 a necessidade disso
#include "minishell.h"
#include <signal.h> //ver se ta na main

// Função auxiliar para imprimir a Árvore (AST) para depuração
// Remover isso mais tarde.
static void	print_ast(t_node *node, int level)
{
	int	i;

	if (!node)
		return ;
	for (i = 0; i < level; i++)
		printf("  ");
	if (node->type == NODE_PIPE)
	{
		printf("PIPE\n");
		print_ast(node->left, level + 1);
		print_ast(node->right, level + 1);
	}
	else if (node->type == NODE_COMMAND)
	{
		printf("COMMAND: ");
		i = 0;
		while (node->command->args && node->command->args[i])
			printf("[%s] ", node->command->args[i++]);
		printf("\n");
	}
}

void	handler_ctr_c(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_node		*ast;
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	(void)argc;
	(void)argv;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handler_ctr_c;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	// create_env_arr(envp); // Vars de ambiente
	(void)envp;
	while (1)
	{
		sigaction(SIGINT, &sa_int, NULL);
		sigaction(SIGQUIT, &sa_quit, NULL);
		input = readline("minishell$ ");
		if (!input)
        {
			printf("exit\n");
			break ;
		}
		if (input[0])
			add_history(input);

		// 2. LEXER: Transforma a string em tokens
		tokens = tokenize(input);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		
		// 3. PARSER: Transforma os tokens na Árvore de Comandos (AST)
		ast = parse_line(&tokens);
		if (!ast)
		{
			free(input);
			free_tokens(tokens);
			continue ;
		}

		// (Apagar dps) Imprime a árvore para ver se tudo funcionou
		printf("--- AST Gerada ---\n");
		print_ast(ast, 0);
		printf("--------------------\n");
		
		// 4. EXECUTOR
		// A função do executor receberia a 'ast'.
		// ex: execute_ast(ast, &env_vars);

		// 5. LIMPEZA DA MEMÓRIA
		free_node(ast);
		free_tokens(tokens);
		free(input);
	}
	// ... (limpeza final das variáveis de ambiente, etc.)
	return (0);
}
