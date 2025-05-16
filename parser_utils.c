/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:41:36 by lalves-d          #+#    #+#             */
/*   Updated: 2025/05/16 16:12:13 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection(t_command *cmd, t_token **curr)
{
	t_redir	*redir;

	if (!*curr || !(*curr)->next)
		return (0);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (0);
	redir->type = (*curr)->type;
	redir->file = ft_strdup((*curr)->next->value);
	if (!redir->file)
		return (free(redir), 0);
	redir->next = cmd->redirs;
	cmd->redirs = redir;
	*curr = (*curr)->next->next;
	return (1);
}

int	init_command_node(t_node **node, t_command **cmd, char ***args)
{
	*node = malloc(sizeof(t_node));
	*cmd = malloc(sizeof(t_command));
	*args = malloc(sizeof(char *) * 1024);
	if (!*node || !*cmd || !*args)
		return (0);
	(*node)->type = NODE_COMMAND;
	(*node)->left = NULL;
	(*node)->right = NULL;
	(*cmd)->redirs = NULL;
	return (1);
}

int	attach_redirection_or_arg(t_command *cmd, t_token **curr,
			char **args, int *argc)
{
	if ((*curr)->type == TOKEN_WORD)
	{
		args[*argc] = ft_strdup((*curr)->value);
		if (!args[*argc])
			return (0);
		(*argc)++;
		return (1);
	}
	return (handle_redirection(cmd, curr));
}

void	fill_command_args(t_command *cmd, char **args, int argc)
{
	args[argc] = NULL;
	cmd->args = args;
}

t_node	*create_pipe_node(t_node *left, t_token **tokens)
{
	t_node	*pipe_node;

	pipe_node = malloc(sizeof(t_node));
	if (!pipe_node)
	{
		free_node(left);
		return (NULL);
	}
	pipe_node->type = NODE_PIPE;
	pipe_node->left = left;
	pipe_node->right = parse_pipeline(tokens);
	if (!pipe_node->right)
	{
		free_node(pipe_node);
		return (NULL);
	}
	return (pipe_node);
}
