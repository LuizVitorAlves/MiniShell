/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:18:55 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/06 12:07:16 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

/*static void	print_ast(t_node *node, int level)
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
}*/

static char	*get_cmd_path_aux(char **paths, char *cmd)
{
	int		i;
	size_t	buffer_size;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		buffer_size = ft_strlen(paths[i]) + ft_strlen(cmd) + 2;
		full_path = malloc(buffer_size);
		if (!full_path)
			return (NULL);
		ft_strlcpy(full_path, paths[i], buffer_size);
		ft_strlcat(full_path, "/", buffer_size);
		ft_strlcat(full_path, cmd, buffer_size);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	*path_var;
	char	**paths;
	char	*full_path;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	path_var = envp[i] + 5;
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	full_path = get_cmd_path_aux(paths, cmd);
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (full_path);
}

static int	free_set_env_var(char ***env, char *key, size_t key_len,
	char **new_entry)
{
	int	i;

	i = -1;
	while ((*env)[++i])
	{
		if (strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=')
		{
			free((*env)[i]);
			(*env)[i] = *new_entry;
			return (0);
		}
	}
	return (i);
}

int	set_env_var(char ***env, const char *key, const char *value)
{
	int		i;
	size_t	key_len;
	char	**new_env;
	int		j;
	char	*new_entry;

	i = -1;
	key_len = strlen(key);
	new_entry = malloc(strlen(key) + strlen(value) + 2);
	if (!new_entry)
		return (1);
	sprintf(new_entry, "%s=%s", key, value);
	i = free_set_env_var(env, (char *)key, key_len, &new_entry);
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (free(new_entry), 1);
	j = -1;
	while (++j < i)
		new_env[j] = strdup((*env)[j]);
	new_env[i] = new_entry;
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

char	**dup_env(char **envp)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1024));
	if (!new_env)
	{
		return (NULL);
	}
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	j = i;
	while (i < j + 1024)
	{
		new_env[i] = NULL;
		i++;
	}
	return (new_env);
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
	(void)sig;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "", 1);
	rl_redisplay();
}

void	handler_ctr_c(int sig, siginfo_t *info, void *notused)
{
	(void)notused;
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		if (info->si_uid)
			rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\r", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	setup_signals(struct sigaction *sa)
{
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = SA_SIGINFO;
	sa->sa_sigaction = handler_ctr_c;
	sigaction(SIGINT, sa, NULL);
	sigaction(SIGQUIT, sa, NULL);
}

static void	handle_empty_or_invalid_input(char *input, t_token *tokens)
{
	if (!input)
		exit(0);
	if (!input[0] || !tokens)
	{
		free(input);
		if (tokens)
			free_tokens(tokens);
	}
}

static void	execute_valid_command(char *input, t_token *tokens, t_node *ast,
	char ***envp_copy)
{
	if (!ast)
	{
		free(input);
		free_tokens(tokens);
		return ;
	}
	executor(ast, envp_copy);
	wait(NULL);
	free_tokens(tokens);
	free(input);
}

void	start_shell_loop(char ***envp_copy)
{
	char				*input;
	char				path_name[1024];
	t_token				*tokens;
	t_node				*ast;
	struct sigaction	sa;

	while (1)
	{
		setup_signals(&sa);
		ft_strlcpy(path_name, "minishell$ ", 12);
		input = readline(path_name);
		if (!input)
			exit(exit_status(-1));
		if (input && input[0])
			add_history(input);
		tokens = tokenize(input);
		if (!tokens)
		{
			handle_empty_or_invalid_input(input, tokens);
			continue ;
		}
		ast = parse_line(&tokens);
		execute_valid_command(input, tokens, ast, envp_copy);
	}
}

/*void	start_shell_loop(char ***envp_copy)
{
	char				*input;
	char				path_name[1024];
	t_token				*tokens;
	t_node				*ast;
	struct sigaction	sa;

	while (1)
	{
		setup_signals(&sa);
		ft_strlcpy(path_name, "minishell$ ", 12);
		input = readline(path_name);
		if (input && input[0])
			add_history(input);
		tokens = tokenize(input);
		if (!tokens)
		{
			handle_empty_or_invalid_input(input, tokens);
			continue ;
		}
		ast = parse_line(&tokens);
		print_ast(ast, 0);
		execute_valid_command(input, tokens, ast, envp_copy);
	}
}*/

int	main(int argc, char *argv[], char **envp)
{
	char	**envp_copy;

	(void)argc;
	(void)argv;
	envp_copy = dup_env(envp);
	get_all_env(envp_copy);
	exit_status(0);
	start_shell_loop(&envp_copy);
	return (0);
}
