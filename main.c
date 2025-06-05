/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:18:55 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 19:22:40 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

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

int	main(int argc, char *argv[], char **envp)
{
	char				*input;
	t_node				*ast;
	char				path_name[1024];
	t_token				*tokens;
	struct sigaction	sa;
	char				**envp_copy;

	envp_copy = dup_env(envp);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler_ctr_c;
	(void)argv;
	(void)argc;
	get_all_env(envp_copy);
	exit_status(0);
	input = NULL;
	while (1)
	{
		ft_strlcpy(path_name, "minishell$ ", 12);
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
		input = readline(path_name);
		if (!input)
			exit(0);
		else
			tokens = tokenize(input);
		tokens = tokenize(input);
		if (input[0])
			add_history(input);
		tokens = tokenize(input);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		ast = parse_line(&tokens);
		if (!ast)
		{
			free(input);
			free_tokens(tokens);
			continue ;
		}
		executor(ast, &envp_copy);
		wait(NULL);
		free_tokens(tokens);
		free(input);
	}
}
