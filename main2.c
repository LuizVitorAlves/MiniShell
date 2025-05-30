#define _DEFAULT_SOURCE //Verificar na 42 a necessidade disso
#include "minishell.h"
#include <signal.h> //ver se ta na main

// Função auxiliar para imprimir a Árvore (AST) para depuração


char **dup_env(char **envp)
{
	int i;
	int j;

	i = 0;
    while (envp[i])
        i++;

    char **new_env = malloc(sizeof(char *) * (i + 1024));
    if (!new_env)
	{
        return NULL;
	}
	i = 0;
	while(envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	j = i;
	while(i < j + 1024)
	{
		new_env[i] = NULL;
		i++;
	}
	return(new_env);
}



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

//Começo
char *get_cmd_path(char *cmd, char **envp)
{
    int i = 0;
    char *path = NULL;
    char **paths = NULL;
    char *full_path;
    size_t buffer_size;

    // Busca a variável PATH no envp
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path = envp[i] + 5; // Pula "PATH="
            break;
        }
        i++;
    }

    if (!path)
        return NULL;

    paths = ft_split(path, ':'); // Função que divide por ":"
    if (!paths)
        return NULL;

    i = 0;
    while (paths[i])
    {
        buffer_size = ft_strlen(paths[i]) + ft_strlen(cmd) + 2; // '/' + '\0'
        full_path = malloc(buffer_size);
        if (!full_path)
            break;

        ft_strlcpy(full_path, paths[i], buffer_size);
        ft_strlcat(full_path, "/", buffer_size);
        ft_strlcat(full_path, cmd, buffer_size);

        if (access(full_path, X_OK) == 0)
        {
           // free_split(paths);
            return full_path;
        }

        free(full_path);
        i++;
    }
	//implementar a free_split
    //free_split(paths);
    return NULL;
}
int set_env_var(char ***env, const char *key, const char *value)
{
    int i = 0;
    size_t key_len = strlen(key);
    char *new_entry = malloc(strlen(key) + strlen(value) + 2); // key=value\0

    if (!new_entry)
        return 1;
    sprintf(new_entry, "%s=%s", key, value);

    // Substitui se já existe
    while ((*env)[i]) {
        if (strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=') {
            free((*env)[i]);
            (*env)[i] = new_entry;
            return 0;
        }
        i++;
    }

    // Cria novo ambiente
    char **new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env) {
        free(new_entry);
        return 1;
    }

    int j = 0;
    while (j < i) {
        new_env[j] = strdup((*env)[j]); // Duplicar as strings antigas
        j++;
    }
    new_env[i] = new_entry;
    new_env[i + 1] = NULL;
	printf("%s", new_env[i]);
    // Libera apenas o array antigo (não as strings)
    free(*env);
    *env = new_env;
    return 0;
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
	char **envp_copy = dup_env(envp);
	sa_quit.sa_flags = 0;
	char	path_name[1024];

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
        executor(tokens, path_name, input, &envp_copy);

		// 5. LIMPEZA DA MEMÓRIA
		free_node(ast);
		free_tokens(tokens);
		free(input);
	}
	// ... (limpeza final das variáveis de ambiente, etc.)
	return (0);
}
