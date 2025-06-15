/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:18:55 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/14 23:11:12 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

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
			shell_exit(exit_status(-1));
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

int	main(int argc, char *argv[], char **envp)
{
	char	***envp_addr;

	(void)argc;
	(void)argv;
	envp_addr = get_main_envp_addr();
	*envp_addr = dup_env(envp);
	get_all_env(*envp_addr);
	exit_status(0);
	start_shell_loop(envp_addr);
	return (0);
}
