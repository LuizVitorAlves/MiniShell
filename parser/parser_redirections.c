/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 01:34:58 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 01:42:54 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*create_redir_node_from_token(t_token *operator_token)
{
	t_redir	*new_node;

	new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return (NULL);
	new_node->type = operator_token->type;
	new_node->file = ft_strdup(operator_token->next->value);
	if (!new_node->file)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

static void	append_redir_to_list(t_command *cmd, t_redir *redir_node)
{
	t_redir	*current;

	if (!cmd->redirs)
	{
		cmd->redirs = redir_node;
	}
	else
	{
		current = cmd->redirs;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = redir_node;
	}
}

int	handle_redirection(t_command *cmd, t_token **curr)
{
	t_redir	*new_redir;

	if (!*curr || !(*curr)->next || (*curr)->next->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
			2);
		exit_status(2);
		return (0);
	}
	new_redir = create_redir_node_from_token(*curr);
	if (!new_redir)
	{
		return (0);
	}
	append_redir_to_list(cmd, new_redir);
	*curr = (*curr)->next->next;
	return (1);
}
