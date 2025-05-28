#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>



char **env_vars = NULL;


// char *get_cmd_path(char *cmd, char **envp)
// {
//     int i = 0;
//     char *path = NULL;
//     char **paths = NULL;
//     char *full_path;

//     // Busca a variável PATH no envp
//     while (envp[i])
//     {
//         if (ft_strncmp(envp[i], "PATH=", 5) == 0)
//         {
//             path = envp[i] + 5; // Pula "PATH="
//             break;
//         }
//         i++;
//     }

//     if (!path)
//         return NULL;

//     paths = ft_split(path, ':'); // Função que divide por ":"
//     if (!paths)
//         return NULL;

//     i = 0;
//     while (paths[i])
//     {
//         full_path = malloc(ft_strlen(paths[i]) + ft_strlen(cmd) + 2); // +1 para '/' e +1 para '\0'
//         if (!full_path)
//             break;

//         ft_strcpy(full_path, paths[i]);
//         ft_strcat(full_path, "/");
//         ft_strcat(full_path, cmd);

//         if (access(full_path, X_OK) == 0)
//         {
//             // Comando encontrado e executável
//             free_split(paths);
//             return full_path;
//         }

//         free(full_path);
//         i++;
//     }

//     free_split(paths); // Libere a matriz criada por ft_split
//     return NULL; // Comando não encontrado
// }




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



//função que modifica as variaveis de ambiente 
// int set_env_var(char ***env, const char *key, const char *value)
// {
//     int i = 0;
//     size_t key_len = strlen(key);
//     char *new_entry = malloc(strlen(key) + strlen(value) + 2); // key=value\0

//     if (!new_entry)
//         return 1;
// 	//sprintf(new_entry, "%s=%s", key, value);
// 	// Verifica se a variável já existe
//     while ((*env)[i]) {
//         if (strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=') {
//             free((*env)[i]);
//             (*env)[i] = new_entry;
//             return 0;
//         }
//         i++;
//     }
// 	// Se não encontrou, cria novo array com espaço extra
//     char **new_env = malloc(sizeof(char *) * (i + 2));
//     if (!new_env) {
//         free(new_entry);
//         return 1;
//     }
// 	int j = 0;
//     while (j < i) {
//         new_env[j] = (*env)[j];
//         j++;
//     }
// 	new_env[i] = new_entry;
//     new_env[i + 1] = NULL;
// 	free(*env);
//     *env = new_env;
// 	return 0;
// }
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

int main(int argc, char *argv[],char **envp)
{
    char	*input;
	char	path_name[1024];
    t_token    *tokens;
	struct sigaction sa;
	struct sigaction sa_quit;
	char **envp_copy = dup_env(envp);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler_ctr_c;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = SIG_IGN;
	(void )argv;
    (void )argc;
	//create_env_arr(&env_vars);
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
        
        // if(ft_strncmp(tokens->value, "exit", 4) == 0 && (tokens->value[4] == ' ' || tokens->value[4] == '\0'))
        //     ft_exit(tokens, input);
		// if(ft_strncmp(tokens->value, "echo", 4) == 0)
		// 	ft_echo(tokens);
		// if(ft_strncmp(tokens->value, "pwd", 3) == 0)
		// 	ft_pwd();
		// if(ft_strncmp(tokens->value, "cd", 2) == 0)
		// 	ft_cd(tokens, path_name);
		// if(ft_strncmp(tokens->value, "export", 6) == 0)
		// 	ft_export(tokens);
		// if(ft_strncmp(tokens->value, "unset", 5) == 0)
		// 	ft_unset(tokens);
		// if(ft_strncmp(tokens->value, "env", 3) == 0)
		// 	ft_env();
		// print_tokens(tokens);

		executor(tokens, path_name, input, &envp_copy);
		free_tokens(tokens);
        free(input);
    }
}
