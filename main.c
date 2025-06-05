#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <wait.h>

char **env_vars = NULL;
char *get_cmd_path_aux(char **paths, char *cmd)
{
    char *full_path;
    int i = 0;
    int buffer_size;

    while (paths[i])
    {
        buffer_size = ft_strlen(paths[i]) + ft_strlen(cmd) + 2;
        full_path = malloc(buffer_size);
        if (!full_path)
            return NULL;
        ft_strlcpy(full_path, paths[i], buffer_size);
        ft_strlcat(full_path, "/", buffer_size);
        ft_strlcat(full_path, cmd, buffer_size);

        if (access(full_path, X_OK) == 0)
            return full_path;

        free(full_path);
        i++;
    }
    return NULL;
}

char *get_cmd_path(char *cmd, char **envp)
{
    int i = 0;
    char *path = NULL;
    char **paths = NULL;
    char *full_path = NULL;

    if (access(cmd, X_OK) == 0)
        return ft_strdup(cmd);

    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path = envp[i] + 5;
            break;
        }
        i++;
    }
    if (!path)
        return NULL;
    paths = ft_split(path, ':');
    if (!paths)
        return NULL;
    full_path = get_cmd_path_aux(paths, cmd);
    free_split(paths);
    return full_path;
}








static void replace_if_env_exist(char ***env, int i, char *key, char *new_entry)
{
    int key_len;

    key_len = ft_strlen(key);
    while ((*env)[i])
    {
        if (strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=') {
            free((*env)[i]);
            (*env)[i] = new_entry;
            return 0;
        }
        i++;
    }   
}
int set_env_var(char ***env, const char *key, const char *value)
{
    int i = 0;
    char *new_entry;

    *new_entry = malloc(strlen(key) + strlen(value) + 2);    
    if (!new_entry)
        return 1;
    sprintf(new_entry, "%s=%s", key, value);
    replace_if_env_exist(&env, i, key, new_entry);
    char **new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env) {
        free(new_entry);
        return 1;
    }
    int j = 0;
    while (j < i) {
        new_env[j] = strdup((*env)[j]);
        j++;
    }
    new_env[i] = new_entry;
    new_env[i + 1] = NULL;
    free(*env);
    *env = new_env;
    return 0;
}

//copia as variaveis de ambiente
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

void	handler_sa_quit(int sig)
{
    
	(void) sig;
	rl_replace_line("", 0);     // limpa o conteúdo da linha atual
    write(STDOUT_FILENO, "", 1); // pular linha        // informa que estamos em uma nova linha
    rl_redisplay();   
}
void	handler_ctr_c(int sig, siginfo_t *info, void *notused)
{
	(void) notused;
    if (sig == SIGINT) {

        rl_replace_line("", 0);     // limpa o conteúdo da linha atual
        write(STDOUT_FILENO, "\n", 1); // pular linha
        rl_on_new_line();           // informa que estamos em uma nova linha
        if (info->si_uid)
            rl_redisplay();             // reexibe o prompt
    }
    else if (sig == SIGQUIT)
    {
        rl_replace_line("", 0);
        write(STDOUT_FILENO, "\r", 1);   // Carriage return para limpar a linha
        rl_on_new_line();
        rl_redisplay();   
    }
}

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
static void free_input(char **input)
{
     free(*input);
    *input = NULL;
}
static void main_aux(char **input, char *path_name, t_token **tokens, t_node **ast)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler_ctr_c;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    *input = readline(path_name);
    if (!(*input))
        exit(0);
    if ((*input)[0])
        add_history(*input);
    *tokens = tokenize(*input);
    if (!(*tokens))
    {
        free_input(*input);
        return;
    }
    *ast = parse_line(tokens);
    if (!(*ast))
    {
        free_input(*input);
        free_tokens(*tokens);
        *tokens = NULL;
    }
}

int main(int argc, char *argv[], char **envp)
{
    char *input = NULL;
    t_node *ast = NULL;
    char path_name[1024];
    t_token *tokens = NULL;
    char **envp_copy;

    (void)argv;
    (void)argc;
    envp_copy = dup_env(envp);
    get_all_env(envp_copy);
    exit_status(0);
    while (1)
    {
        ft_strlcpy(path_name, "minishell$ ", 12);
        main_aux(&input, path_name, &tokens, &ast);
        if (ast)
            executor(ast, &envp_copy);
        wait(NULL);
        free_tokens(tokens);
        tokens = NULL;
        free(input);
        input = NULL;
    }
}

