/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:07:52 by uviana-b          #+#    #+#             */
/*   Updated: 2025/06/14 22:53:21 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_exit(int exit_code)
{
	char	**g_envp_copy;

	g_envp_copy = NULL;
	if (g_envp_copy)
		free_string_array(g_envp_copy);
	clear_history();
	exit(exit_code);
}

void	handle_empty_or_invalid_input(char *input, t_token *tokens)
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

void	execute_valid_command(char *input, t_token *tokens, t_node *ast,
		char ***envp_copy)
{
	if (!ast)
	{
		free(input);
		free_tokens(tokens);
		return ;
	}
	executor(ast, envp_copy);
	free_node(ast);
	free_tokens(tokens);
	free(input);
}
